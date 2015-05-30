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
#include <iomanip>

#include "../include/crypt/aes.h"
//#include "../include/crypt/key.h"

#define CMD_MORE_DATA   0x55
#define CMD_NO_DATA     0xAA
#define ACK             0x66

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
        cout << "bytes written: " /q<< byteswritten << endl;
#endif // DEBUG
    }
}

void terminal::on_le_command_returnPressed()
{
    // get all the contents of the output so we can append the new output
    // before re-displaying it
    QString buffer = this->ui->te_output->toPlainText();
    QString message;
    // get the user input
    message = this->ui->le_command->text();

    QByteArray message_buffer = message.toLatin1();

    // check if the user wishes to quit
    if (!message.compare("exit")) {
        serversock->write(message.toLatin1());
        serversock->waitForBytesWritten();
        serversock->close();
        QApplication::quit();
    }

    buffer.append(message);
    this->ui->te_output->clear();
    this->ui->te_output->appendPlainText(buffer);
    this->ui->te_output->appendPlainText("mintty> ");
    this->ui->le_command->clear();

    // send the user input to the server
    serversock->write(message_buffer, message_buffer.size());
    serversock->waitForBytesWritten();

    // get the control command from the server
    QByteArray controlcommand;
    if (!serversock->waitForReadyRead()) {
        serversock->close();
        QApplication::quit();
    }

    controlcommand = serversock->read(1);
    if (controlcommand.size() != 1) {
        serversock->close();
        QApplication::quit();
    }

    if (controlcommand.at(0) == CMD_MORE_DATA) {
        controlcommand.clear();
        controlcommand.append(ACK);
        if (serversock->write(controlcommand, controlcommand.size()) == -1) {
            serversock->close();
            QApplication::quit();
        }
        serversock->waitForBytesWritten();
    }

    /* get the number of bytes to receive */
    QByteArray nbyteoutput;
    if (!serversock->waitForReadyRead()) {
        serversock->close();
        QApplication::quit();
    }
    nbyteoutput = serversock->read(16);
    if (nbyteoutput.size() == 0) {
        serversock->close();
        QApplication::quit();
    }

    unsigned long long int nbytes_toreceive = 0;

    /* since we are receiving the size in separate bytes, we need
     * to concatenate it
     */
    for (int i = (nbyteoutput.size() - 1); i >= 0; i--) {
        nbytes_toreceive |= nbyteoutput[i];
        if (i != 0)
            nbytes_toreceive <<= 8;
    }

    /* calculate the number of 16-bit data blocks to receive */
    unsigned int ndatablocks;

    if (nbytes_toreceive != 0)
        ndatablocks = (nbytes_toreceive + 16) / 16;
    else
        ndatablocks = 0;

    cout << "number of 16-bit datablocks to receive " <<
            ndatablocks << endl;

    /* get all the datablocks */
    for (int datablockindex = 0; datablockindex < ndatablocks; datablockindex++) {
        /* get one of the datablocks */
        QByteArray datablock;
        cout << "waiting" << endl;
        if (!serversock->waitForReadyRead()) {
            serversock->close();
            QApplication::quit();
        }
        datablock = serversock->read(16);
        if (datablock.size() != 16) {
            serversock->close();
            QApplication::quit();
        }
        for (int i = 0; i < datablock.size(); i++) {
            cout << datablock[i] << " ";
        }
        cout << endl;
        cout << "done waiting and receiving" << endl;

        cout << "sending ack" << endl;
        /*if (!serversock->waitForReadyRead()) {
            serversock->close();
            QApplication::quit();
        }*/

        controlcommand.clear();
        controlcommand = serversock->read(1);
        if (controlcommand.size() != 1) {
            serversock->close();
            QApplication::quit();
        }

        if (controlcommand.at(0) == CMD_MORE_DATA) {
            controlcommand.clear();
            controlcommand.append(ACK);
            if (serversock->write(controlcommand, controlcommand.size()) == -1) {
                serversock->close();
                QApplication::quit();
            }
            serversock->waitForBytesWritten();
        }

        cout << "ack sent" << endl;
    }




}
