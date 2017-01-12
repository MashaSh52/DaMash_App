#include "imageprovider.h"
#include "stdexcept"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

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

    if (role == Qt::DisplayRole)
    {
        if (ddd->type != IMAGE)
        {
           HData* temp = (HData* )ddd->data;
           return temp->name;
        }
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
    return parent_pointer->count;
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
        query.prepare("SELECT * FROM images WHERE PID = :id ORDER BY NUMBER");
    }

    query.bindValue(":id", data->id);
    query.exec();

    while(query.next())
    {
        auto id = query.value("id").toUInt(); //работать, скорее всего, не будет, потому что могут быть проблемы с версиями qt
        auto comment = query.value("comment").toString();
        QStringList tags = query.value("tags").toStringList();
        auto number = query.value("number").toInt();

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
            auto path = query.value("path").toString();
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
//    if(cf == 2)
//        this->addNewCourse(2, "Разработка корпоративных приложений");

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
