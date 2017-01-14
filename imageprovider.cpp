#include "imageprovider.h"
#include "stdexcept"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QUrl>
#include <imageprocessing.h>

ImageProvider::ImageProvider(QString nameOfDB, QObject *parent)
{
    Q_UNUSED(parent);
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(nameOfDB);
    if(!database.open()){
        throw std::invalid_argument("Database is not open!");
    }

    fetchAll(QModelIndex());
    //выгружает один слой из БД: семестры/предметы/картинки

}

ImageProvider::~ImageProvider()
{
    database.close();
}

bool ImageProvider::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    DataWrapper* dw = dataForIndex(index);

    switch(dw->type){
    case TERM:{
        createNewTerm(value.toString());
        break;
    }
    case COURSE:
    {
        QModelIndex fatherTerm = this->parent(index);
        createNewCourse(fatherTerm, value.toString());
        break;
    }
    default:
    {
        IData* data = value.value<IData*>();
        QModelIndex fatherCourse = this->parent(index);
        createNewImage(fatherCourse, data->path, data->comments, data->tags);
    }
    }

    //DataWrapper* tempDW = dataForIndex(index);
    emit dataChanged(index, index);
    return true;
}


QVariant ImageProvider::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const DataWrapper*ddd =  dataForIndex(index);

    if (role == Qt::DisplayRole) // text view
    {
        if (ddd->type != IMAGE)
        {
           HData* temp = (HData* )ddd->data;
           return temp->name;
        }
        else return ddd->number;
    }

    if (role == Qt::DecorationRole && ddd->type == IMAGE)
    {
        IData* temp = (IData* )ddd->data;
        return QUrl::fromLocalFile(temp->path);
    }

    return QVariant();
}

int ImageProvider::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return 1;

}

int ImageProvider::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return dw.count;
    const DataWrapper* parent_pointer = dataForIndex(parent);
    return parent_pointer->children.size();
}

QModelIndex ImageProvider::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0)
        return QModelIndex();

    if(!parent.isValid())
        return createIndex(row,column, dw.children[row]);

    const DataWrapper* parent_pointer = dataForIndex(parent);
    if (parent_pointer->type == IMAGE)
        return QModelIndex();

    if(parent_pointer->children.size() <= row)
        return QModelIndex();

    return createIndex(row, column, parent_pointer->children[row]);
}

QModelIndex ImageProvider::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    const DataWrapper* child_pointer = dataForIndex(child);

    if(!child_pointer->parent)
        return QModelIndex();

    return createIndex(child_pointer->parent->number,0,static_cast<void*>(child_pointer->parent));

}

void ImageProvider::fetchMore(const QModelIndex &parent)
{
    fetchAll(parent);
}

bool ImageProvider::canFetchMore(const QModelIndex &parent) const
{
    const DataWrapper* data = dataForIndex(parent);
    return data->children.size() < data->count;
}

bool ImageProvider::hasChildren(const QModelIndex &parent) const
{
    const DataWrapper* d = dataForIndex(parent);
    if (d->count != 0)
        return true;
    else
        return false;
}

bool ImageProvider::addNewTerm(QString nameOfTerm)
{
    if(!this->index(0,0,QModelIndex()).isValid())
        return 1;
    QModelIndex i = createIndex(0,0, &dw);

    for(auto it = dw.children.begin(); it != dw.children.end(); ++it)
    {
        HData* insideData = (HData*)(*it)->data;
        if(insideData->name == nameOfTerm)
            return 1;
    };

    qint16 t = this->rowCount(i);
    beginInsertRows(QModelIndex(), t, t);

    HData* insideData = new HData;
    DataWrapper* newItem = new DataWrapper;

    insideData->comments = "";
    //insideData->tags = {};

    newItem->data = insideData;
    newItem->parent = &dw;
    newItem->type = TERM;

    dw.children.append(newItem);
    dw.count++;

    endInsertRows();

    QModelIndex index = createIndex(t,0,dw.children[t]);
    //DataWrapper* dddd = dataForIndex(index);
    this->setData(index, nameOfTerm, Qt::EditRole);
    return 0;

}

bool ImageProvider::addNewCourse(QModelIndex currentIndex, QString nameOfCourse)
{
    //TODO: проверка на существование курса
    if(!currentIndex.isValid())
        return 1;

    DataWrapper* curTerm = dataForIndex(currentIndex);    
    for(auto it=curTerm->children.begin(); it != curTerm->children.end(); ++it)
    {
        HData* insideData = (HData*)(*it)->data;
        if(insideData->name == nameOfCourse)
            return 1;
    }

    int t = this->rowCount(currentIndex);
    beginInsertRows(currentIndex, t, t);

    HData* insideData = new HData;
    DataWrapper* newCourse = new DataWrapper;

    insideData->comments = "";
    //insideData->tags = {};

    newCourse->data = insideData;
    newCourse->parent = curTerm;
    newCourse->type = COURSE;

    curTerm->children.append(newCourse);
    curTerm->count++;

    endInsertRows();

    QModelIndex index = createIndex(t,0,curTerm->children[t]);
    this->setData(index, nameOfCourse, Qt::EditRole);
    return 0;

}

bool ImageProvider::addNewImage(QModelIndex currentIndex, QString path, QString comments, QStringList tags)
{

    if(!currentIndex.isValid())
        return 1;

    DataWrapper* curCourse = dataForIndex(currentIndex);   
    for(auto it=curCourse->children.begin(); it != curCourse->children.end(); ++it)
    {
        IData* insideData = (IData*)(*it)->data;
        if(insideData->path == path)
            return 1;
    }
    int t = this->rowCount(currentIndex);
    beginInsertRows(currentIndex, t, t);

    IData* insideData = new IData;
    DataWrapper* newImage = new DataWrapper;

    insideData->comments = comments;
    insideData->tags = tags;
    insideData->path = path;

    QVariant toCreateImgPost = QVariant::fromValue(insideData);

    newImage->data = insideData;
    newImage->parent = curCourse;

    curCourse->children.append(newImage);
    curCourse->count++;

    endInsertRows();

    QModelIndex index = createIndex(t,0,curCourse->children[t]);
    this->setData(index, toCreateImgPost, Qt::EditRole);
    return 0;

}

bool ImageProvider::deleteElement(QModelIndex currentIndex)
{
    if(!currentIndex.isValid())
        return 1;

    DataWrapper* curData = dataForIndex(currentIndex);
    this->fetchMore(currentIndex);


    QModelIndex parent = QModelIndex();
    if(curData->type != TERM)
        parent = currentIndex.parent();
    DataWrapper* parentData = dataForIndex(parent);

    if(curData->children.size() > 0)
    {
        for (int i = 0; i < curData->children.size();++i)
        {
            //COURSES
            DataWrapper* firstFloor = curData->children[i];
            QModelIndex firstIndex = index(i,0,currentIndex);
            this->fetchMore(firstIndex);

            if(firstFloor->children.size() > 0)
            {
                //IMAGES
                beginRemoveRows(firstIndex, 0, firstFloor->count-1);
                for(int k = 0; k < firstFloor->count;++k)
                {
                    QSqlQuery queryForDelete;
                    queryForDelete.prepare("DELETE FROM IMAGES WHERE ID = :ID");
                    queryForDelete.bindValue(":ID", firstFloor->children[k]->id);
                    queryForDelete.exec();

                    firstFloor->children.removeAt(k);
                    firstFloor->count--;
                }
                endRemoveRows();

            }

            beginRemoveRows(currentIndex,0,curData->count-1);
            for(int k = 0; k < curData->count;++k)
            {
                QSqlQuery queryForDelete;
                queryForDelete.prepare("DELETE FROM RELATIONSHIOS WHERE ID = :ID");
                queryForDelete.bindValue(":ID", curData->children[k]->id);
                queryForDelete.exec();

                curData->children.removeAt(k);
                curData->count--;
             }
            endRemoveRows();
        }
    }

    beginRemoveRows(parent,curData->number-1,curData->number-1);

    QSqlQuery queryForDelete;
    queryForDelete.prepare("DELETE FROM RELATIONSHIOS WHERE ID = :ID");
    queryForDelete.bindValue(":ID", curData->id);
    queryForDelete.exec();

    parentData->children.removeAt(curData->number-1);
        curData->count--;
    endRemoveRows();
    return 0;

}

QVariantList ImageProvider::getChildrenIndexesOfItem(QModelIndex currentIndex)
{
    if(!currentIndex.isValid())
        return QVariantList();
    QVariantList res;
    res.push_back(currentIndex);

    QModelIndex ind;
    DataWrapper* currentItem = dataForIndex(currentIndex);
    if(currentItem->count > 0)
    {
        for(int i = 0; i < currentItem->children.size(); ++i)
        {
            DataWrapper* firstFloor = currentItem->children[i];
            QModelIndex firstIndex = index(i,0,currentIndex);
            res.push_back(firstIndex);

            if(firstFloor->count > 0)
            {
                for(int j = 0; j < firstFloor->children.size(); ++j)
                {
                    //DataWrapper* secondFloor = firstFloor->children[j];
                    QModelIndex secondIndex = index(j, 0, firstIndex);
                    res.push_back(secondIndex);
                }
            }
        }
    }
    return res;
}

qint16 ImageProvider::whatElementIsIt(QModelIndex currentIndex)
{
    DataWrapper* currentData = dataForIndex(currentIndex);
    if(currentData->type == TERM)
        return 1;
    if(currentData->type == COURSE)
        return 2;
    if(currentData->type == IMAGE)
        return 3;
    return 0;
}

bool ImageProvider::setTransitIndex(QModelIndex ind)
{
    if (!ind.isValid())
        return 1;
    transitIndex = ind;
    return 0;
}

QModelIndex ImageProvider::getTransitIndex()
{
    if (!transitIndex.isValid())
        return QModelIndex();
    return transitIndex;
}


void ImageProvider::fetchAll(const QModelIndex &parent)
{
    DataWrapper* data = dataForIndex(parent);
    data->children.clear();
    QSqlQuery query;

    if (data->type != COURSE) //Если из первой таблицы
    {
        query.prepare("SELECT * FROM relationships WHERE PID = :id ORDER BY NUMBER");
    }
    else
    {   // with images
        query.prepare("SELECT * FROM IMAGES WHERE PID = :id ORDER BY NUM_OF_IMG");
    }

    query.bindValue(":id", data->id);
    query.exec();

    beginInsertRows(parent,0,data->count-1);
    while(query.next())
    {
        auto id = query.value("id").toUInt(); //работать, скорее всего, не будет, потому что могут быть проблемы с версиями qt
        auto comment = query.value("comment").toString();
        QStringList tags = query.value("tags").toStringList();
        int number;
        if (data->type != COURSE)
            number = query.value("number").toInt();
        else
            number = query.value("num_of_img").toInt();


        switch (data->type){
        case ROOT:
        case TERM:{
            auto type = query.value("type").toInt();
            auto name = query.value("name").toString();
            data->children.append(
                        new DataWrapper{id, (element_type)type,
                                        new HData{name,comment,tags},
                                        number, data, {}, getChildrenCount(element_type(type),id)
                        });
            break;
        }
        case COURSE: {
            auto path = query.value("path_to_file").toString();
            data->children.append(
                        new DataWrapper{id, IMAGE,
                                        new IData{path,comment,tags},
                                        number, data, {}, getChildrenCount(IMAGE,id)

                        });
            break;
        }
        default: break;

        }//switch

    } //while

    data->count = data->children.size();

    endInsertRows();

}

const DataWrapper *ImageProvider::dataForIndex(const QModelIndex &index) const
{
    if(!index.isValid()) //в корне
        return &dw;

    return static_cast<DataWrapper*> (index.internalPointer());
}

DataWrapper *ImageProvider::dataForIndex(const QModelIndex &index)
{
    if(!index.isValid())
        return &dw;

    return static_cast<DataWrapper*> (index.internalPointer());
}

int ImageProvider::getChildrenCount(element_type type, int pid) const
{
    QSqlQuery query;

    switch(type){
    case ROOT:
    case TERM: {
        query.prepare("SELECT COUNT (*) AS RES FROM RELATIONSHIPS WHERE PID=:id");
        break;
    }
    case COURSE: {
        query.prepare("SELECT COUNT (*) AS RES FROM IMAGES WHERE PID=:id");
        break;
    }
    case IMAGE:
        return 0;
    default: break;
    }
    query.bindValue(":id",pid);

    query.exec();
    query.next();

    int count = query.value("RES").toInt();
    return count;


}

bool ImageProvider::createNewCourse(QModelIndex &parent, const QString nameOfCourse)
{
    if(!parent.isValid())
        return 1;
    DataWrapper* fatherTerm = dataForIndex(parent);

    // Working with database
    QSqlQuery curIdAndNumber;

    curIdAndNumber.prepare("SELECT MAX(NUMBER) AS NUM FROM RELATIONSHIPS WHERE TYPE = 2 AND PID = :PID");
    curIdAndNumber.bindValue(":PID",fatherTerm->id);
    curIdAndNumber.exec();
    curIdAndNumber.next();
    qint32 num = curIdAndNumber.value(0).toInt();

    curIdAndNumber.prepare("SELECT MAX(ID) AS I FROM RELATIONSHIPS");
    curIdAndNumber.exec();
    curIdAndNumber.next();
    qint32 id = curIdAndNumber.value(0).toInt();


    QSqlQuery queryToInsert;

    queryToInsert.prepare("INSERT INTO RELATIONSHIPS VALUES(:ID,:PID,:NAME,:COMMENT,:TYPE,:NUMBER)");
    queryToInsert.bindValue(":ID", id+1);
    queryToInsert.bindValue(":PID", fatherTerm->id);
    queryToInsert.bindValue(":NAME", nameOfCourse);
    queryToInsert.bindValue(":COMMENT", "");
    queryToInsert.bindValue(":TYPE", 2);
    queryToInsert.bindValue(":NUMBER", num+1);

    queryToInsert.exec();


    //Working with tree of model
    DataWrapper* newCourse = fatherTerm->children[fatherTerm->count - 1]; //сейчас у нас уже есть там созданный узел
    newCourse->id = id+1;

    HData* tempHD = new HData;
    tempHD = (HData*)newCourse->data;
    tempHD->name = nameOfCourse;
    tempHD->comments = "";
    newCourse->data= tempHD;


    newCourse->count = 0;
    newCourse->children.clear();
    newCourse->number = num + 1;
    return 0;

}

bool ImageProvider::createNewImage(QModelIndex &parent, const QString path, const QString comment, const QStringList tags)
{
    if(!parent.isValid())
        return 1;
    DataWrapper* fatherCourse = dataForIndex(parent);

    // Working with database
    QSqlQuery curIdAndNumber;

    curIdAndNumber.prepare("SELECT MAX(NUM_OF_IMG) AS NUM FROM IMAGES WHERE PID = :PID");
    curIdAndNumber.bindValue(":PID",fatherCourse->id);
    curIdAndNumber.exec();
    curIdAndNumber.next();
    qint32 num = curIdAndNumber.value(0).toInt();

    curIdAndNumber.prepare("SELECT MAX(ID) AS I FROM IMAGES");
    curIdAndNumber.exec();
    curIdAndNumber.next();
    qint32 id = curIdAndNumber.value(0).toInt();


    QSqlQuery queryToInsert;

    queryToInsert.prepare("INSERT INTO IMAGES VALUES(:ID, :PID, :NUM_OF_IMG,:PATH_TO_FILE,:COMMENT,:TAGS)");
    queryToInsert.bindValue(":ID", id+1);
    queryToInsert.bindValue(":PID", fatherCourse->id);
    queryToInsert.bindValue(":NUM_OF_IMG", num+1);
    queryToInsert.bindValue(":PATH_TO_FILE", path);
    queryToInsert.bindValue(":COMMENT", comment);
    queryToInsert.bindValue(":TAGS", tags.join(","));

    queryToInsert.exec();


    //Working with tree of model
    DataWrapper* newImg = fatherCourse->children[fatherCourse->count - 1]; //сейчас у нас уже есть там созданный узел
    newImg->id = id+1;

    IData* tempID = new IData;
    tempID = (IData*)newImg->data;
    tempID->path = path;
    tempID->comments = comment;
    tempID->tags = tags;

    newImg->data= tempID;
    newImg->type = IMAGE;

    newImg->count = 0;
    newImg->children.clear();
    newImg->number = num + 1;
    return 0;
}

bool ImageProvider::createNewTerm(QString nameOfTerm)
{
    // Working with database
    QSqlQuery curIdAndNumber;

    curIdAndNumber.prepare("SELECT MAX(NUMBER) AS NUM FROM RELATIONSHIPS WHERE TYPE = 1");
    curIdAndNumber.exec();
    curIdAndNumber.next();
    qint32 num = curIdAndNumber.value(0).toInt();

    curIdAndNumber.prepare("SELECT MAX(ID) AS I FROM RELATIONSHIPS");
    curIdAndNumber.exec();
    curIdAndNumber.next();
    qint32 id = curIdAndNumber.value(0).toInt();


    QSqlQuery queryToInsert;

    queryToInsert.prepare("INSERT INTO RELATIONSHIPS VALUES(:ID,:PID,:NAME,:COMMENT,:TYPE,:NUMBER)");
    queryToInsert.bindValue(":ID", id+1);
    queryToInsert.bindValue(":PID", 0);
    queryToInsert.bindValue(":NAME", nameOfTerm);
    queryToInsert.bindValue(":COMMENT", "");
    queryToInsert.bindValue(":TYPE", 1);
    queryToInsert.bindValue(":NUMBER", num+1);

    queryToInsert.exec();


    //Working with tree of model
    DataWrapper* newTerm = dw.children[dw.count - 1]; //сейчас у нас уже есть там созданный узел
    newTerm->id = id+1;
    HData* tempHD = new HData;
    tempHD = (HData*)newTerm->data;
    tempHD->name = nameOfTerm;
    tempHD->comments = "";
    newTerm->data= tempHD;

    newTerm->count = 0;
    newTerm->children.clear();
    newTerm->number = num + 1;
    return 0;
}
