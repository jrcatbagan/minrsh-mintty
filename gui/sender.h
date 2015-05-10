#ifndef SENDER_H
#define SENDER_H

#include <QMainWindow>

namespace Ui {
class sender;
}

class sender : public QMainWindow
{
    Q_OBJECT

public:
    explicit sender(QWidget *parent = 0);
    ~sender();

private:
    Ui::sender *ui;
};

#endif // SENDER_H
