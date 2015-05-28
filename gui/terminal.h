//
// File: terminal.h
// Date: 2015, May 09
//
// Copyright (C) 2015, Irene Cho <irenechoster@gmail.com>
// Copyright (C) 2015, Jarielle Catbagan <jrcatbagan@ca.rr.com>

#ifndef TERMINAL_H
#define TERMINAL_H

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include "defines.h"

namespace Ui {
class terminal;
}

class terminal : public QDialog
{
    Q_OBJECT

private slots:
    void settext(QString message);
    void establish_comm(QString ipaddressliteral, QString portliteral);

    void on_le_command_returnPressed();
    void handledata();

public:
    explicit terminal(QWidget *parent = 0);
    ~terminal();
    QTcpSocket *serversock;
    enum program_state_t program_state;
    unsigned int ndatablocks;
    unsigned int datablockindex;
    QString messageoutput;


private:
    Ui::terminal *ui;
};

#endif // TERMINAL_H
