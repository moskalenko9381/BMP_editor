#include <mygraphicview.h>

MyGraphicView::MyGraphicView(QWidget* parent):QGraphicsView (parent)
{
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    group = new QGraphicsItemGroup();

}

void MyGraphicView::update(QPixmap pixmap){
    scene = new QGraphicsScene();
  //  fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    scene->addPixmap(pixmap);
    this->setScene(scene);
}

void MyGraphicView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    foreach( QGraphicsItem *item, scene->items()) {
       if(item->group() == group) {
          delete item;
       }
    }
}


MyGraphicView::~MyGraphicView() {
}

void MyGraphicView::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton){
        QPoint pos = mapToScene(event->pos()).toPoint();
            x_click = pos.x();
            y_click = pos.y();
            mouse_pressed = true;
    }
}

void MyGraphicView::mouseMoveEvent(QMouseEvent* event){
    if (mouse_pressed && (button_pressed == ROUND || button_pressed == FLOOD)){
       this->deleteItemsFromGroup(group);
        group = new QGraphicsItemGroup();
        QPen  outlinePen = penColor;
           outlinePen.setWidth(thick);
           group->addToGroup(scene->addEllipse(x_click - radius,
                         y_click - radius,
                         radius*2,
                         radius*2,
                         outlinePen,
                         QBrush(Qt::NoBrush)));
        scene->addItem(group);
    }
}



void MyGraphicView::mouseReleaseEvent(QMouseEvent *event){
   QPoint pos = mapToScene(event->pos()).toPoint();
       x_second = pos.x();
       y_second = pos.y();
    mouse_pressed = false;
    this->deleteItemsFromGroup(group);
    button_pressed = 0;
    emit selection();
}

