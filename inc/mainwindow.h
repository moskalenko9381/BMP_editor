#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mygraphicview.h"
#include "image.h"
#include "info1.h"
#include "answers.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QMessageBox>
#include <QFileDialog>


#define ROUND 1
#define FLOOD 2
/*#define LINE  3
#define BLACK 4
#define POLY 5 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selection();
    void on_answers_triggered();
    void on_open_triggered();
    void on_save_triggered();
    void on_task2_clicked();
    void on_task3_clicked();
    void on_floodFill_clicked();
    void on_task4_clicked();
    void on_task1_clicked();
    void on_info_triggered();

private:
    Ui::MainWindow *ui;
    Image* img;
    answers* help;
    QColor color;
    QColor border;
    QColor penThick;
    MyGraphicView *picture;
    Info* info1;
    QString str;
    int button = 0;
    int valRGB;
    int radius;
    int thick;
    int Yparts;
    int Xparts;
};
#endif // MAINWINDOW_H

