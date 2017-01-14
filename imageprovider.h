#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QObject>
#include<QAbstractItemModel>
#include <QSqlDatabase>
#include <QStringList>

enum element_type {ROOT, TERM = 1, COURSE, IMAGE};

/*
 * СТРУКТУРА БД:
 *      ТАБЛИЦА RELATIONSHIPS:
 *          ID, PID, NAME, COMMENT, TYPE, NUMBER
 *      ТАБЛИЦА IMAGES:
 *          ID, PID, NUM_OF_IMG, PATH_TO_FILE, COMMENT, TAGS
 * */

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
    int count;
};

Q_DECLARE_METATYPE(IData*)

QModelIndex transitIndex;

class ImageProvider : public QAbstractItemModel
{
    Q_OBJECT
public:
    ImageProvider(QString nameOfDB, QObject *parent = nullptr);
    ~ImageProvider();

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    void fetchMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;
    bool hasChildren(const QModelIndex &parent) const;

    // === FOR INTERFACE ===

    /*
     * addNewTerm - функция добавления нового семестра
     *
     * args:
     *  QString nameOfTerm - название нового семестра
     *
    */
    Q_INVOKABLE bool addNewTerm(QString nameOfTerm);
    /*
     * addNewCourse - функция добавления нового семестра
     *
     * args:
     *  QModelIndex currentIndex - индекс узла, на котором произошел клик (TERM),
     *  QString nameOfCourse - название нового курса
     *
    */
    Q_INVOKABLE bool addNewCourse(QModelIndex currentIndex, QString nameOfCourse);
    /*
     * addNewImage - функция добавления нового семестра
     *
     * args:
     *  QModelIndex currentIndex - индекс узла, на котором произошел клик (COURSE)
     *  QString path - путь к фото,
     *  QString comments - комментарий,
     *  QStringList tags - теги
     *
    */
    Q_INVOKABLE bool addNewImage(QModelIndex currentIndex, QString path, QString comments, QStringList tags);

   /*
    * bool deleteElement - удаляет выбранный по индексу семестр/курс/фотографию
    * QModelIndex currentIndex - индекс элемента, который следует удалить
    *
    **/
    Q_INVOKABLE bool deleteElement(QModelIndex currentIndex);

    /*
     * getChildrenIndexesOfItem - возвращает список индексов всех потомков элемента
     *
     * QModelIndex currentIndex - индекс элемента, чьих потомков мы хотим получить
     *
     **/
    Q_INVOKABLE QVariantList getChildrenIndexesOfItem(QModelIndex currentIndex);

    /*
     * whatTermIsIt - возвращает значение элемента по индексу, на который кликнули
     *
     * 0 - если ROOT
     * 1 - если TERM
     * 2 - если COURSE
     * 3 - если IMAGE
     *
     * */
    Q_INVOKABLE qint16 whatElementIsIt(QModelIndex currentIndex);
    Q_INVOKABLE bool setTransitIndex(QModelIndex ind);
    Q_INVOKABLE QModelIndex getTransitIndex();
private:
    QSqlDatabase database;
    DataWrapper dw {0, ROOT, nullptr, 0, nullptr, {}, -1};

    //Забирает все элементы данного уровня - всех потомков
    void fetchAll(const QModelIndex &parent);
    const DataWrapper* dataForIndex(const QModelIndex &index) const;
    DataWrapper* dataForIndex(const QModelIndex &index);
    int getChildrenCount(element_type type, int pid) const;

    bool createNewTerm(const QString nameOfTerm);
    bool createNewCourse(QModelIndex &parent, const QString nameOfCourse);
    bool createNewImage(QModelIndex &parent, const QString path, const QString comment, const QStringList tags);



};

#endif // IMAGEPROVIDER_H
