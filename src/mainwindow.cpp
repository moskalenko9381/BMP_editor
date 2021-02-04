#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <iostream>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new Image();

    picture = new MyGraphicView();

    info1 = new Info();

    help = new answers();

    connect(picture, SIGNAL(selection()), this, SLOT(selection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_answers_triggered()
{
   help->exec();
}

void MainWindow::on_open_triggered()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Выберите файл для открытия", "/home/user", "*.bmp");
           if (str == nullptr)
               return;
              int error = img->loadImage(str.toLocal8Bit().constData());
              if (error == -1){
                  QMessageBox::critical(this, "Ошибка", "Возникла неизвестная ошибка при чтении файла");
                  return;
              }
              else if (error == -2){
                  QMessageBox::critical(this, "Ошибка", "Ширина или высота изображения не могут быть больше 10.000 пикселей");
                  return;
              }

              else if (error == -4){
                  QMessageBox::critical(this, "Ошибка", "Количество пикселей на цвет не равно 24");
                  return;
              }
              else if (error == -5){
                  QMessageBox::critical(this, "Ошибка", "Изображение должно быть без сжатия");
                  return;
              }
               QPixmap pixmap = img->getPixmap();
               picture->update(pixmap);
               ui->gridLayout->addWidget(picture);
               QFileInfo fileInfo(str);
               info1->height = img->bih.height;
               info1->width = img->bih.width;
               info1->size = img->bfh.filesize;
               info1->isReadable = fileInfo.isReadable();
               info1->isWritable = fileInfo.isWritable();
               info1->path = fileInfo.absoluteFilePath();
               info1->name = fileInfo.fileName();
}


void MainWindow::on_save_triggered(){
    if (img->bih.width == 0 || img->bih.width == 0){
        QMessageBox::critical(this, "Oops!","Нет изначального изображения");
    return;
    }
    QString str = QFileDialog::getSaveFileName(nullptr, "Введите имя файла для сохранения", "/home/user", "*.bmp");
           if (str == nullptr)
               return;
          int error = img->saveImage(str.toLocal8Bit().constData());
          if (error != 0)
               QMessageBox::critical(this, "Oops!","Ошибка при сохранении файла");
                 }


void MainWindow::selection(){
    if (button == ROUND || button == FLOOD) {

        if (picture->y_click <= 0 || picture->x_click <= 0
                || picture->y_click + radius + thick >= img->bih.height
                || picture->x_click - radius - thick < 0
                || picture->y_click - radius - thick < 0
                || picture->x_click + radius + thick >= img->bih.width)
        {
            QMessageBox::critical(this, "Oops!","Радиус слишком большой");
        return;
        }


        if (button == ROUND && thick == 0)
             img->drawCircle(img->bih.height - picture->y_click,  picture->x_click ,  radius, color);


        if (button == ROUND && thick != 0)
                img->drawWithThick(img->bih.height - picture->y_click,  picture->x_click, radius, radius + thick, color);


        if (button == FLOOD){
            img->flood(img->bih.height - picture->y_click,  picture->x_click,  radius, color);

            if (thick != 0)
                 img->drawWithThick(img->bih.height - picture->y_click,  picture->x_click, radius, radius + thick, penThick);
        }

    }
    picture->update(img->getPixmap());
    button = 0;
        }




void MainWindow::on_task3_clicked()
{
    if (ui->red->isChecked())
        str = "red";
    else if (ui->blue->isChecked())
            str = "blue";

    else if(ui->green->isChecked())
            str = "green";

    valRGB = ui->spinThick->value();
    if (str != "blue" && str !="green" && str != "red")
          QMessageBox::critical(this, "Oops!","Введите цвет: синий, красный или зелёный");
    if (img->bih.width == 0 || img->bih.height == 0)
         QMessageBox::critical(this, "Oops!","Для начала загрузите изображение");
    img->rgbFilter(str,valRGB);
    QPixmap pixmap = img->getPixmap();
    picture->update(pixmap);
}


void MainWindow::on_task2_clicked()
{
    if (img->bih.width == 0 || img->bih.height == 0){
         QMessageBox::critical(this, "Oops!","Для начала загрузите изображение");
            return;
}
    color = QColorDialog::getColor(Qt::white, this, "Выберите цвет окружности");
            if (!color.isValid()){
                    QMessageBox::critical(this, "Oops!","Вы не выбрали цвет");
                    return;
                  }
    picture->penColor = color;
    radius = ui->radius->value();
    picture->radius = radius;
    thick = ui->thickRound->value();
    picture->thick = thick;
    button = ROUND;
    picture->button_pressed = ROUND;
}


void MainWindow::on_floodFill_clicked(){
    if (img->bih.width == 0 || img->bih.height == 0){
         QMessageBox::critical(this, "Oops!","Для начала загрузите изображение");
         return;
    }
    color = QColorDialog::getColor();
            if (!color.isValid()){
                    QMessageBox::critical(this, "Oops!","Вы не выбрали цвет");
                    return;
                  }

            if (ui->thickRound->value() != 0){
    penThick = QColorDialog::getColor();
                    if (!penThick.isValid()){
                            QMessageBox::critical(this, "Oops!","Вы не выбрали цвет");
                            return;
                   }
                     picture->penThick = penThick;
            }

     picture->penColor = color;
     picture->radius = radius;
     picture->thick = thick;
    radius = ui->radius->value();
    thick = ui->thickRound->value();
    button = FLOOD;
    picture->button_pressed = FLOOD;
}



void MainWindow::on_task4_clicked(){
    if (img->bih.width == 0 || img->bih.height == 0){
         QMessageBox::critical(this, "Oops!","Для начала загрузите изображение");
             return;
        }
        color = QColorDialog::getColor(Qt::white, this, "Выберите цвет сетки");
                if (!color.isValid()){
                     QMessageBox::critical(this, "Oops!","Вы не выбрали цвет");
                        return;
            }
                Xparts = ui->Xparts->value();
                Yparts = ui->Yparts->value();
                thick = ui->thicklines->value();


     if (thick == 1){
      for (int i = 0; i < Xparts - 1; i++){

            img->drawLine(img->bih.width/Xparts + i*(img->bih.width/Xparts),  0 ,
                      img->bih.width/Xparts + i*(img->bih.width/Xparts), img->bih.height - 1 ,color);
}

     for (int i = 0; i < Yparts -1; i++)
            img->drawLine(0, img->bih.height/Yparts + i*(img->bih.height/Yparts), img->bih.width - 1,
                          img->bih.height/Yparts + i*(img->bih.height/Yparts), color);
     }

     if (thick > 1){
          for (int i = 0; i < Xparts -1; i++){
              for (int j = -thick/2; j < thick/2; j++)
                  img->drawLine(img->bih.width/Xparts + i*(img->bih.width/Xparts) + j,  0 ,
                            img->bih.width/Xparts + i*(img->bih.width/Xparts) + j, img->bih.height - 1 ,color);

        for (int i = 0; i < Yparts; i++){

                 img->drawLineWithThick(0, img->bih.height/Yparts + i*(img->bih.height/Yparts), img->bih.width - 1,
                               img->bih.height/Yparts + i*(img->bih.height/Yparts), thick, color);
     }
          }
}
       picture->update(img->getPixmap());

}



void MainWindow::on_task1_clicked()
{
    if (img->bih.width == 0 || img->bih.height == 0){
         QMessageBox::critical(this, "Oops!","Для начала загрузите изображение");
         return;
    }
    color = QColorDialog::getColor(Qt::white, this, "Выберите цвет искомых прямоугольников");
            if (!color.isValid()){
                    QMessageBox::critical(this, "Oops!","Вы не выбрали цвет");
                    return;
                  }
    border = QColorDialog::getColor(Qt::white, this, "Выберите цвет рамки");
                    if (!border.isValid()){
                            QMessageBox::critical(this, "Oops!","Вы не выбрали цвет");
                            return;
                          }

                 if (img->check(color) == 0){
                      QMessageBox::critical(this, "Oops!","Такого цвета на картинке нет");
                        return;
                 }
                 int frame = ui->frame->value();
                 int c =  img->rectangle_coordinates(color,border, frame);
                 ui->spin->setValue(c);
                 picture->update(img->getPixmap());
}


void  MainWindow::on_info_triggered(){
  if (img->bih.width == 0 || img->bih.height == 0){
         QMessageBox::critical(this, "Oops!","Для начала загрузите изображение");
         return;
    }
  info1->height = img->bih.height;
  info1->width = img->bih.width;
  info1->size = img->bfh.filesize;
  info1->writeInfo();
  info1->exec();
}

