//
// File: terminal.cpp
// Date: 2015, May 09
//
// Copyright (C) 2015, Irene Cho <irenechoster@gmail.com>
// Copyright (C) 2015, Jarielle Catbagan <jrcatbagan@ca.rr.com>

#include "terminal.h"
#include "ui_terminal.h"
#include <QString>
#include <QHostAddress>
#include <QTcpSocket>
#include <iostream>

using namespace std;

terminal::terminal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::terminal)
{
    ui->setupUi(this);
    this->ui->te_output->setReadOnly(true);
}

terminal::~terminal()
{
    delete ui;
}

void terminal::settext(QString message)
{
    this->ui->te_output->appendPlainText(message);
    this->ui->te_output->appendPlainText("mintty> ");
}

void terminal::establish_comm(QString ipaddressliteral, QString portliteral)
{
    QHostAddress ipaddress(ipaddressliteral);
    quint16 port = portliteral.toLongLong();
    QByteArray data;
    data.append(0x55);
    data.append(0xAA);
    data.append(0x55);
    data.append(0xAA);

    serversock = new QTcpSocket;

    serversock->connectToHost(ipaddress, port);
    if (serversock->waitForConnected()) {
#ifdef DEBUG
        quint64 byteswritten = serversock->write(data, data.size());
#else
        serversock->write(data, data.size());
#endif // DEBUG
       if (!serversock->waitForBytesWritten(data.size()))
            cout << "error";
#ifdef DEBUG
        cout << "bytes written: " << byteswritten << endl;
#endif // DEBUG
    }
}

void terminal::on_le_command_returnPressed()
{
    QString buffer = this->ui->te_output->toPlainText();
    QString message;
    message.clear();
    message = this->ui->le_command->text();

    if (!message.compare("exit")) {
        serversock->write(message.toUtf8());
        serversock->waitForBytesWritten();
        serversock->close();
        QApplication::quit();
    }

    buffer.append(message);
    this->ui->te_output->clear();
    this->ui->te_output->appendPlainText(buffer);
    this->ui->te_output->appendPlainText("mintty> ");
    this->ui->le_command->clear();

    cout << message.toStdString() << endl;

    serversock->write(message.toUtf8());
    serversock->waitForBytesWritten();
}
