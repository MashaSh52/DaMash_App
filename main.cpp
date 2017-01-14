#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "imageprovider.h"
#include <QQmlContext>
//#include <QApplication>
//#include <QTreeView>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ImageProvider model("/home/skt/appForLections/db");
 //   ImageProvider model("/home/skt/appForLections/DaMash_App/db");
    engine.rootContext()->setContextProperty("dbModel", &model);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));


//    QApplication app(argc,argv);
//    QTreeView view;
//    ImageProvider model("/home/skt/appForLections/db");
//    view.setModel(&model);

//    view.show();

//    model.addNewTerm("Спецкурс ЗИ");
//    model.addNewTerm("Спецкурс АДМ");

    //model.deleteTerm(4);



    return app.exec();
}


