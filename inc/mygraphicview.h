#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QImage>
#include <QColor>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPoint>

#define ROUND 1
#define FLOOD 2
#define LINE 3

class MyGraphicView : public QGraphicsView
{
    Q_OBJECT


public:
     QPoint* pos;
     int button_pressed = 0;
     int thick;
     int x_click;
     int y_click;
     int x_second;;
     int y_second;
     int radius;
      QGraphicsScene* scene;
     QColor penColor;
     QColor penThick;
    void update(QPixmap pixmap);
     MyGraphicView(QWidget* parent = nullptr);
    ~MyGraphicView();

signals:
    void selection();

protected slots:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    QGraphicsItemGroup* group;
    QPixmap pixmap;
    bool mouse_pressed = false;
    void deleteItemsFromGroup(QGraphicsItemGroup* group);
};

#endif // MYGRAPHICVIEW_H

