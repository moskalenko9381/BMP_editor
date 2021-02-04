#include "answers.h"
#include "ui_answers.h"

answers::answers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::answers)
{
    ui->setupUi(this);
}

answers::~answers()
{
    delete ui;
}

