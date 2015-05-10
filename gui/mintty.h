#ifndef MINTTY_H
#define MINTTY_H

#include <QMainWindow>
#include <QString>
#include "terminal.h"

namespace Ui {
class mintty;
}

class mintty : public QMainWindow
{
    Q_OBJECT

signals:
    void sendmessage(QString message);
    void send_comm_params(QString ipaddress, QString port);

public:
    explicit mintty(QWidget *parent = 0);
    ~mintty();
    terminal *mintty_terminal;

private slots:
    void on_btn_connect_clicked();

private:
    Ui::mintty *ui;
};

#endif // MINTTY_H
