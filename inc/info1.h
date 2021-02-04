#ifndef INFO1_H
#define INFO1_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class Info;
}

class Info : public QDialog
{
    Q_OBJECT
public:
    explicit Info(QWidget *parent = nullptr);
    ~Info();
    void writeInfo();
    QString name;
    QString path;
    int height;
    int width;
    int size;
    bool isReadable;
    bool isWritable;
private:
    Ui::Info *ui;
};

#endif // INFO1_H

