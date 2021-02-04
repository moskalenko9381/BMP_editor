#include "info1.h"
#include "ui_info1.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
}

Info::~Info()
{
    delete ui;
}

void Info::writeInfo(){
    ui->width->setText(QString::number(width));
    ui->height->setText(QString::number(height));
    ui->size->setText(QString::number(size));
    ui->name->setText(name);
    ui->path->setText(path);
    if (isWritable)
        ui->write->setText("Yes");
    else
         ui->write->setText("No");
    if (isReadable)
        ui->read->setText("Yes");
    else
         ui->read->setText("No");
}

