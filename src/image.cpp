#include "image.h"
#include <math.h>
#include <algorithm>
#include "ui_mainwindow.h"
#define PI 3.14159265
#define PI2 1.57079632679489661923

int Image::loadImage(const char *file) {
    FILE* f = fopen(file, "rb");
    if (!f)
        return -1;
    fread(&bfh, sizeof(bfh), 1, f);
    fread(&bih, sizeof(bih), 1, f);

    if (bih.height > 10000 || bih.width > 10000)
        return -2;
    if (bih.bitsPerPixel != 24)
        return -4;
    if (bih.compression != 0)
        return -5;

    size_t padding = 0;
    if ((bih.width*3) % 4)
        padding = 4 - (bih.width*3) % 4;

    rgb = new RGB*[bih.height];

    for (int i = 0; i < bih.height; i++){
        rgb[i] = new RGB [bih.width + 1];
    }

    fseek(f, long(bfh.pixelArrOffset), SEEK_SET);

    for (int i = 0; i < bih.height; i++) {
        int j;
        for  (j = 0;  j < bih.width; j++){
            fread(&rgb[i][j], sizeof(RGB), 1, f);
        }
        if (padding)
            fread(&rgb[i][j], padding, 1, f);
    }

    fclose(f);
    return 0;
}

int Image::saveImage(const char* file){
    FILE* f = fopen(file, "wb");
    if (!f)
        return -1;

    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    size_t padding = 0;
    if ((bih.width*3) % 4)
        padding = 4 - (bih.width*3) % 4;

    fseek(f, long(bfh.pixelArrOffset), SEEK_SET);

    for (int i = 0; i < bih.height; i++) {
        int j;
        for (j = 0; j < bih.width; j++){
            fwrite(&rgb[i][j], sizeof(RGB), 1, f);
        }
        if (padding)
            fwrite(&rgb[i][j], padding, 1, f);
    }
    fclose(f);
    return 0;
}



QPixmap Image::getPixmap(){
     QImage *image = new QImage(bih.width, bih.height, QImage::Format_RGB666); //создаем изображение
     QColor pixel;
     for (int i = bih.height - 1;   i >= 0; i--) {
                 for (int j = 0; j < bih.width;   j++) {
                 pixel.setRed(rgb[i][j].red);
                 pixel.setGreen(rgb[i][j].green);
                 pixel.setBlue(rgb[i][j].blue);
                 image->setPixel(j, bih.height - i-1, pixel.rgb());
                 }
     }
 return QPixmap::fromImage(*image);
}


void Image::rgbFilter(QString color, int f){
    for (int i = 0; i <  bih.height; i++) {
        for  (int j = 0;  j < bih.width; j++){
           if (color == "blue")
               rgb[i][j].blue = f;
           if (color == "green")
               rgb[i][j].green = f;
           if (color == "red")
               rgb[i][j].red = f;
         }
    }
}


void Image::drawCircle(int x0, int y0, int radius, QColor color){
        int x = 0;
        int y = radius;
        int delta = 1 - 2 * radius;
        int error = 0;
        while(y >= 0) {
            rgb[x0 + x][y0 + y].green = color.green();
            rgb[x0 + x][y0 + y].blue = color.blue();
            rgb[x0 + x][y0 + y].red = color.red();
            rgb[x0 + x][y0 - y].green = color.green();;
            rgb[x0 + x][y0 - y].blue = color.blue();
            rgb[x0 + x][y0 - y].red = color.red();
            rgb[x0 - x][y0 + y].green = color.green();
            rgb[x0 - x][y0 + y].blue = color.blue();
            rgb[x0 - x][y0 + y].red = color.red();
            rgb[x0 - x][y0 - y].green = color.green();
            rgb[x0 - x][y0 - y].blue = color.blue();
            rgb[x0 - x][y0 - y].red = color.red();
            error = 2 * (delta + y) - 1;
            if(delta < 0 && error <= 0) {
                ++x;
                delta += 2 * x + 1;
                continue;
            }
            error = 2 * (delta - x) - 1;
            if(delta > 0 && error > 0) {
                --y;
                delta += 1 - 2 * y;
                continue;
            }
            ++x;
            delta += 2 * (x - y);
            --y;
        }
    }




void Image::flood(int n_x, int n_y, int radius, QColor color){

for (int i = n_x - radius; i <= n_x+radius; i++){

    for(int j = n_y-radius; j <= n_y+radius; j++){

        if( (sqrt(pow(i-n_x,2)+pow(j-n_y,2))) <= radius)
                    if (i>0  && j>0 ){
                            rgb[i][j].green = color.green();
                            rgb[i][j].red = color.red();
                            rgb[i][j].blue = color.blue();
                                     }
                            }
            }
}


void Image::drawWithThick(int n_x, int n_y, int small, int big, QColor color){

    for (int i = n_x - big; i <= n_x + big; i++){

        for(int j = n_y - big; j <= n_y + big; j++){

            if( (sqrt(pow(i-n_x,2) + pow(j-n_y,2))) <= big && (sqrt(pow(i-n_x,2) + pow(j-n_y,2))) > small)

                        if (i>0  && j>0 ){

                                rgb[i][j].green = color.green();
                                rgb[i][j].red = color.red();
                                rgb[i][j].blue = color.blue();
                                         }
                                }
                }
}


void Image::drawLine(int x1, int y1, int x2, int y2, QColor color){
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    rgb[y2][x2].blue = color.blue();
    rgb[y2][x2].green = color.green();
    rgb[y2][x2].red = color.red();
    while(x1 != x2 || y1 != y2)
   {
        rgb[y1][x1].blue = color.blue();
        rgb[y1][x1].green = color.green();
        rgb[y1][x1].red = color.red();
        const int error2 = error * 2;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}



void Image::drawLineWithThick(int x1, int y1, int x2, int y2, int thick, QColor color){
    if (abs(y2-y1) > abs(x2-x1)){
        if (x1-thick/2 < 0 || x2-thick/2 < 0 || x1+thick/2 >= bih.width || x2+thick/2 >= bih.height)
            return;
        for (int i = -thick/2; i < thick/2; i++)
            drawLine(x1+i, y1, x2+i, y2, color);
    }
    else{
        if (y1-thick/2 < 0 || y2-thick/2 < 0 || y1+thick/2 >= bih.height || y2+thick/2 >= bih.height)
            return;
        for (int i = -thick/2; i < thick/2; i++)
            drawLine(x1, y1+i, x2, y2+i, color);
    }
}



int Image::compare(RGB a, QColor color){
    if (abs(a.green - color.green()) <= 25  &&
           abs(a.red - color.red()) <= 25 &&
            abs(a.blue - color.blue()) <= 25)
        return 1;
    return 0;
}



int Image::check(QColor color) {
        int k = 0;
        int i, j;
        for (i = 0; i < bih.height; i++)
            for (j = 0; j < bih.width; j++){
                if (compare(rgb[i][j],color))
                    k++;
            }
        return k;
            }


void Image::finded(int i, int j, int** a, int** output, int* index){
    int x = bih.height;
    int y = bih.width;

    int flagC = 0;
    int flagR = 0;
    int m, n;
    for (m = i; m < x; m++){
        if (a[m][j] == 1) {
            flagR = 1;
            break;
        }

        if (a[m][j] == 5)
            continue;

        for (n = j; n < y; n++){
            if (a[m][n] == 1){
                flagC = 1;
                flagR = 1;
                break;
            }
            a[m][n] = 5;
        }
    }

    if (flagR == 1)
        output[*index][2] = m-1;
    else
        output[*index][2] = m;

    if (flagC == 1)
        output[*index][3] = n-1;
    else
        output[*index][3] = n;
}



int Image::rectangle_coordinates(QColor color, QColor border, int frame) {
    int** output = new int*[20000];
    for (int i = 0; i < 20000; i++)
        output[i] = new int[4];
    int index = -1;
    output[0][0] = 1;

    const int n = bih.height;
    const int m = bih.width;
    int** a = new int *[n+1];
    for (int i = 0; i < n; i++)
        a[i] = new int[m+1];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (compare(rgb[i][j],color))
                a[i][j] = 0;
             else
                a[i][j] = 1;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (a[i][j] == 0){
                index++;
                output[index][0] = i;
                output[index][1] = j;
                finded(i, j, a, output, &index);
            }
        }
    }

      for(int j = 0; j <= index; j++){
        for (int i = 0; i < frame; i++){

            if(output[j][1] <= 0 || output[j][1]  >= bih.width - 1||
                    output[j][2] - frame + 1 <= 0 || output[j][2] - frame + 1  >= bih.height -1||
                    output[j][3]  <= 0 || output[j][3]  >= bih.width - 1
                    )
                        continue;


        if(output[j][1] - frame + 1 <= 0 || output[j][1] - frame  + 1 >= bih.width - 1 ||
                output[j][2] <= 0 || output[j][2] >= bih.height -1||
                output[j][0]  <= 0 || output[j][0] >= bih.height - 1
                )
                    continue;



        if(output[j][3] + frame -1 <= 0 || output[j][3] + frame -1 >= bih.width -1 ||
                output[j][2] <= 0 || output[j][2] >= bih.height-1 ||
                output[j][0]  <= 0 || output[j][0] >= bih.height-1
                )
                    continue;



        if(output[j][1]  <= 0 || output[j][1]  >= bih.width - 1 ||
                output[j][3]  < 0 || output[j][3] >= bih.width - 1||
                output[j][0] + frame -1 <= 0 || output[j][0] + frame -1 >= bih.height- 1
                )
                    continue;

        drawLine(output[j][1], output[j][2] - i, output[j][3], output[j][2] - i, border);


        drawLine(output[j][1] - i, output[j][2], output[j][1] - i, output[j][0], border);

        drawLine(output[j][3] + i, output[j][2], output[j][3] + i, output[j][0], border);

        drawLine(output[j][1], output[j][0] + i, output[j][3], output[j][0] + i, border);

}
}
     for (int i = 0; i < 20000; i++)
        delete [] output[i];
     delete [] output;

     for (int i = 0; i < n; i++)
        delete [] a[i];
     delete [] a;
     return index+1;
}
