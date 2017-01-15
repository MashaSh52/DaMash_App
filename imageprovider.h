#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QObject>
#include<QAbstractItemModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QUrl>
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

extern QModelIndex transitIndex;
extern QUrl transitURL;
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
     * addNewCourse - функция добавления нового курса
     *
     * args:
     *  QModelIndex currentIndex - индекс узла, на котором произошел клик (TERM),
     *  QString nameOfCourse - название нового курса
     *
    */
    Q_INVOKABLE bool addNewCourse(QModelIndex currentIndex, QString nameOfCourse);
    /*
     * addNewImage - функция добавления нового фото
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
    Q_INVOKABLE bool setTransitURL(QUrl url);
    Q_INVOKABLE QUrl getTransitURL();
    /*
     * rotateImage - функция поворота изображения
     * QString pathToImage - путь к изображению,
     * QString newPath - путь для сохранения измененного изображения,
     * qreal angle - угол поворота в градусах
     * */
    Q_INVOKABLE QUrl rotateImage(QString pathToImage, QString newPath, qreal angle);
    /*
     * cropImage - функция обрезки изображения
     * QString pathToImage - путь к изображению,
     * QString newPath - путь для сохранения измененного изображения,
     * int x1, int y1 - координаты левого верхнего угла вырезаемого прямоугольника,
     * int x2, int y2 - координаты правого нижнего угла
     * */
    Q_INVOKABLE QUrl cropImage(QString pathToImage, QString newPath, int x1, int y1, int x2, int y2);
    /*
     * makeBlackAndWhiteImage - функция перевода изображния в черно-белый формат
     * QString pathToImage - путь к изображению,
     * QString newPath - путь для сохранения измененного изображения
     * */
    Q_INVOKABLE QUrl makeBlackAndWhiteImage(QString pathToImage, QString newPath);
    /*
     * printImage - функция печати изображения
     * QString pathToImage - путь к изображению
     * */
    Q_INVOKABLE bool printImage(QString pathToImage);

    // Получить комментарии, соотвествующие текущему элементу
    Q_INVOKABLE QString getComment(QModelIndex currentIndex);
    // Получить теги, соответствующие текущему элементу
    Q_INVOKABLE QString getTags(QModelIndex currentIndex);

    // Задать комментарий для элемента
    Q_INVOKABLE bool setComment(QModelIndex currentIndex, QString comment);
    // Задать теги для элемента
    Q_INVOKABLE bool setTags(QModelIndex currentIndex, QString tags);
private:
    QSqlDatabase database;
    DataWrapper dw {0, ROOT, nullptr, 0, nullptr, {}, -1};

    void fetchAll(const QModelIndex &parent);
    const DataWrapper* dataForIndex(const QModelIndex &index) const;
    DataWrapper* dataForIndex(const QModelIndex &index);
    int getChildrenCount(element_type type, int pid) const;

    bool createNewTerm(const QString nameOfTerm);
    bool createNewCourse(QModelIndex &parent, const QString nameOfCourse);
    bool createNewImage(QModelIndex &parent, const QString path, const QString comment, const QStringList tags);
    bool deleteAllChildren(QModelIndex currentIndex);



};

#endif // IMAGEPROVIDER_H
