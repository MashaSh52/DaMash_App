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
    cf=0;
    //Лезем в БД и подгружаем все данные
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

    DataWrapper* tempDW = dataForIndex(index);
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
    //TODO: проверка на существование семестра

    if(!this->index(0,0,QModelIndex()).isValid())
        return 1;
    QModelIndex i = createIndex(0,0, &dw);
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
    DataWrapper* dddd = dataForIndex(index);
    this->setData(index, nameOfTerm, Qt::EditRole);

}

bool ImageProvider::addNewCourse(qint16 termNumber, QString nameOfCourse)
{
    //TODO: проверка на существование курса

    QModelIndex curIndex = this->index(termNumber,0,QModelIndex());
    if(!curIndex.isValid())
        return 1;

    DataWrapper* curTerm = dataForIndex(curIndex);
    int t = this->rowCount(curIndex);
    beginInsertRows(curIndex, t, t);

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

}

bool ImageProvider::addNewImage(qint16 termNumber, qint16 courseNumber, QString path, QString comments, QStringList tags)
{
    QModelIndex termIndex = this->index(termNumber,0,QModelIndex());
    QModelIndex curIndex = this->index(courseNumber,0,termIndex);
    if(!curIndex.isValid())
        return 1;

    DataWrapper* curCourse = dataForIndex(curIndex);
    int t = this->rowCount(curIndex);
    beginInsertRows(curIndex, t, t);

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

}

bool ImageProvider::deleteTerm(qint16 termNumber)
{
    QModelIndex rootIndex = QModelIndex();
    DataWrapper *root = dataForIndex(rootIndex);

    QModelIndex deletedTerm = this->index(termNumber,0,rootIndex);
    this->fetchMore(deletedTerm);
    DataWrapper * term = dataForIndex(deletedTerm);


    while(term->count != 0)
        deleteCourse(termNumber, term->count-1);


    beginRemoveRows(rootIndex, termNumber, termNumber);
    root->children.removeAt(termNumber);
    root->count--;
    endRemoveRows();

    QSqlQuery queryForDelete;
    queryForDelete.prepare("DELETE FROM RELATIONSHIPS WHERE ID = :ID");
    queryForDelete.bindValue(":ID", term->id);
    queryForDelete.exec();

    return true;


}

bool ImageProvider::deleteCourse(qint16 termNumber, qint16 courseNumber)
{
    QModelIndex termIndex = this->index(termNumber,0,QModelIndex());
    QModelIndex deletedCourse = this->index(courseNumber, 0, termIndex);
    if(!termIndex.isValid())
        return 0;

    DataWrapper *term = dataForIndex(termIndex);
    this->fetchMore(deletedCourse);
    DataWrapper *course= dataForIndex(deletedCourse);

    while(course->count != 0)
        deleteImage(termNumber,courseNumber,course->count-1);


    beginRemoveRows(termIndex, courseNumber, courseNumber);
    term->children.removeAt(courseNumber);
    term->count--;
    endRemoveRows();

    QSqlQuery queryForDelete;
    queryForDelete.prepare("DELETE FROM RELATIONSHIPS WHERE ID = :ID");
    queryForDelete.bindValue(":ID", course->id);
    queryForDelete.exec();

    return true;

}

bool ImageProvider::deleteImage(qint16 termNumber, qint16 courseNumber, qint16 imgNumber)
{
    QModelIndex termIndex = this->index(termNumber,0,QModelIndex());
    QModelIndex courseIndex = this->index(courseNumber,0,termIndex);
    QModelIndex imgIndex = this->index(imgNumber, 0, courseIndex);

    if(!imgIndex.isValid())
        return 1;

    DataWrapper *term = dataForIndex(termIndex);
    DataWrapper *course = dataForIndex(courseIndex);
    DataWrapper *img = dataForIndex(imgIndex);



    beginRemoveRows(courseIndex, imgNumber, imgNumber);
    course->children.removeAt(imgNumber);
    course->count--;
    endRemoveRows();

    QSqlQuery queryForDelete;
    queryForDelete.prepare("DELETE FROM IMAGES WHERE ID = :ID");
    queryForDelete.bindValue(":ID", img->id);
    queryForDelete.exec();

    return true;

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
                    DataWrapper* secondFloor = firstFloor->children[j];
                    QModelIndex secondIndex = index(j, 0, firstIndex);
                    res.push_back(secondIndex);
                }
            }
        }
    }
    return res;
}


void ImageProvider::fetchAll(const QModelIndex &parent)
{
    cf = cf+1;
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
//    if(cf == 2)
//        this->getChildrenIndexesOfTerm(0);

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
