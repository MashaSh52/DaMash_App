#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "imageprovider.h"
#include <QQmlContext>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ImageProvider model("/home/skt/appForLections/db");
 //   ImageProvider model("/home/skt/appForLections/DaMash_App/db");
    engine.rootContext()->setContextProperty("dbModel", &model);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    return app.exec();
}


