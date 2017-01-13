#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

/*
 * ВСЕ ФУНКЦИИ ВОЗВРАЩАЮТ QImage
 */

/*
 * rotateImage - функция поворота изображения
 * args:
 *  pathToImage - путь к фото
 *  angle - угол поворота в градусах (-360..360) !!! real
 */
QImage rotateImage(QString pathToImage, qreal angle);
/*
 * cropImage - функция обрезки изображения
 *  QString pathToImage - путь к фото,
 *  int x1 - абсцисса левого верхнего угла,
 *  int y1 - ордината левого верхнего угла,
 *  int x2 - абсцисса правого нижнего,
 *  int y2 - ордината правого нижнего
 */
QImage cropImage(QString pathToImage, int x1, int y1, int x2, int y2);

/*
 * makeBlackAndWhiteImage - функция перевода изображения в черно белый вид
 *  QString pathToImage - путь к фото
 **/

QImage makeBlackAndWhiteImage(QString pathToImage);

#endif // IMAGEPROCESSING_H
