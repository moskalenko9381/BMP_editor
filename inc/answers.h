#ifndef ANSWERS_H
#define ANSWERS_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class answers;
}

class answers : public QDialog
{
    Q_OBJECT

public:
    explicit answers(QWidget *parent = nullptr);
    ~answers();
private:
    Ui::answers *ui;
};

#endif // ANSWERS_H

