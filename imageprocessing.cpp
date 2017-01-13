#include <QImage>
#include <QTransform>
#include <imageprocessing.h>
#include <QRect>


QImage rotateImage(QString pathToImage, qreal angle)
{
    QImage defaultImage(pathToImage);
    QTransform rotating;
    rotating.rotate(angle);

    QImage newImage;
    newImage = defaultImage.transformed(rotating);
    return newImage;
}

QImage cropImage(QString pathToImage, int x1, int y1, int x2, int y2)
{
    QRect rect(x1, y1, x2, y2);
    QImage original(pathToImage);
    QImage cropped = original.copy(rect);
    return cropped;
}

QImage makeBlackAndWhiteImage(QString pathToImage)
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
    return bw;
}
