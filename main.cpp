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
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    ImageProvider model("/home/skt/appForLections/db");



   /* QApplication app(argc,argv);
    QTreeView view;
    ImageProvider model("/home/skt/appForLections/db");
    view.setModel(&model);
    view.show();*/
    model.addNewTerm("TEMP TERM");


    return app.exec();
}
