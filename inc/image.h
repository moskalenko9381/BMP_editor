#ifndef IMAGE_H
#define IMAGE_H
#include <QColorDialog>
#include <QMessageBox>
#include "mygraphicview.h"

class Image{

#pragma pack (push, 1)
typedef struct
{
    unsigned short signature; //BM
    unsigned int filesize;      //размер файла в байтах
    unsigned short reserved1;  //0
    unsigned short reserved2;   //0
    unsigned int pixelArrOffset;   //сколько байтов до начала пиксельного массива
} BitmapFILEHeader;

typedef struct
{
    unsigned int headerSize;   //размер структуры
    int width;        //широта в пикселях
    int height;  //высота
    unsigned short planes;       //количество плоскостей = 1
    unsigned short bitsPerPixel;   //кол-во бит на пиксель
    unsigned int compression;  //тип сжатия. BI_RGB
    unsigned int imageSize;  //размер в байтах, 0
    unsigned int xPixelsPerMeter;  //гор разрешение  устройства (пиксель на метр)
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;  //кол-во используемых цветов из таблицы (если 0, то очень много)
    unsigned int importantColorCount;  //кол-во важных цветов (если 0, то все важны)
} BitmapINFOHeader;

typedef struct RGB
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} RGB;


typedef struct point
{
     int x;
     int y;
}point;

#pragma pack(pop)

public:
    BitmapFILEHeader bfh;
    BitmapINFOHeader bih;
    RGB** rgb;
    RGB** small;
    QPixmap getPixmap();
    int loadImage(const char*);
    int saveImage(const char*);
    void rgbFilter(QString, int);
    void drawCircle(int, int, int, QColor);
    void flood(int,int,int, QColor);
    void drawWithThick(int, int, int, int, QColor);
    void drawLine(int, int, int, int, QColor);
    void drawLineWithThick(int,int,int,int,int,QColor);
    int rectangle_coordinates(QColor, QColor, int);
    int compare(RGB, QColor);
    int check(QColor color);
    void finded(int i, int j, int** a, int** output, int* index);
};

#endif // IMAGE_H

