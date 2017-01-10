#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QObject>
#include<QAbstractItemModel>
#include <QSqlDatabase>
#include <QStringList>

// index(), parent(), rowCount(), columnCount(), and data().

enum element_type {ROOT, TERM = 1, COURSE, IMAGE};

//RELATIONSHIPS: id, pid, name, comment, type, number
//IMAGES:id,pid,numOfImg,nameOfFile,Comment,tags

struct IData{//таблица IMAGES
    QString path;
    QString comments;
    QStringList tags;
};

struct HData{//таблица RELATIONSHIPS
    QString name;
    QString comments;
    QStringList tags;
};

struct DataWrapper{
    qint16 id;
    element_type type;
    void* data; //HData or IData
    int number;
    DataWrapper* parent;
    QList<DataWrapper*> children;
    int count; //количество потомков
};

class ImageProvider : public QAbstractItemModel
{
    Q_OBJECT
public:
    ImageProvider(QString nameOfDB, QObject *parent = nullptr);
    ~ImageProvider();

    //bool setData(const QModelIndex &index, QVariant &value, int role);


    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    void fetchMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;

private:
    QSqlDatabase database;
    DataWrapper dw {0, ROOT, nullptr, 0, nullptr, {}, -1};

    //Забирает все элементы данного уровня - всех потомков
    void fetchAll(const QModelIndex &parent);
    const DataWrapper* dataForIndex(const QModelIndex &index) const;
    DataWrapper* dataForIndex(const QModelIndex &index);
    int getChildrenCount(element_type type, int pid) const;



};

#endif // IMAGEPROVIDER_H
