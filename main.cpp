#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "imageprovider.h"
//#include <QApplication>
//#include <QTreeView>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    ImageProvider model("/home/skt/appForLections/db");

    engine.rootContext()->setContextProperty("mymodel", &model);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));


//    QApplication app(argc,argv);
//    QTreeView view;
//    ImageProvider model("/home/skt/appForLections/db");
//    view.setModel(&model);

//    view.show();

//    model.addNewTerm("Спецкурс ЗИ");
//    model.addNewTerm("Спецкурс АДМ");


    return app.exec();
}

