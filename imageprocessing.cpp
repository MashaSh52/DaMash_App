#include <QImage>
#include <QPainter>
#include <QTransform>
#include <imageprocessing.h>
#include <QRect>
#include <QPrinter>
#include <QPrintDialog>
#include <QUrl>


QUrl rotateImage(QString pathToImage, QString newPath, qreal angle)
{
    QImage defaultImage(pathToImage);
    QTransform rotating;
    rotating.rotate(angle);

    QImage newImage;
    newImage = defaultImage.transformed(rotating);
    newImage.save(newPath);
    return QUrl::fromLocalFile(newPath);
}

QUrl cropImage(QString pathToImage, QString newPath, int x1, int y1, int x2, int y2)
{
    QRect rect(x1, y1, x2, y2);
    QImage original(pathToImage);
    QImage cropped = original.copy(rect);
    cropped.save(newPath);
    return QUrl::fromLocalFile(newPath);
}

QUrl makeBlackAndWhiteImage(QString pathToImage, QString newPath)
{
    QImage original(pathToImage);
    QImage bw = original;
    QSize sizeImage = original.size();
    int width = sizeImage.width(), height = sizeImage.height();

    QRgb color;

    for (int f1=0; f1<width; f1++) {
        for (int f2=0; f2<height; f2++) {
            color = bw.pixel(f1,f2);
            int gray = (qRed(color) + qGreen(color) + qBlue(color))/3;
            bw.setPixel(f1, f2, qRgb(gray, gray, gray));
        }
    }
    bw.save(newPath);
    return QUrl::fromLocalFile(newPath);
}

/*
 * Ошибка на инициализации диалога. Попытаться сделать с qml
 * */
bool printPhoto(QString pathToImage)
{
    QPrinter printer;
    QPrintDialog* dlg = new QPrintDialog(&printer,0);

    if(dlg->exec() == QDialog::Accepted) {
                    QImage img(pathToImage);
                    QPainter painter(&printer);
                    painter.drawImage(QPoint(0,0),img);
                    painter.end();
            }

    delete dlg;
    return 0;
}
