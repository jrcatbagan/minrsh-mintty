//
// File: mintty.cpp
// Date: 2015, May 09
//
// Copyright (C) 2015, Irene Cho <irenechoster@gmail.com>
// Copyright (C) 2015, Edward Casilio <ed.rambozzo@gmail.com>
// Copyright (C) 2015, Jarielle Catbagan <jrcatbagan@ca.rr.com>

#include "mintty.h"
#include "ui_mintty.h"
#include "terminal.h"
#include <QString>
#include <iostream>

using namespace std;

mintty::mintty(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mintty)
{
    ui->setupUi(this);
    mintty_terminal = new terminal;

    QObject::connect(this, SIGNAL(sendmessage(QString)), this->mintty_terminal,
                     SLOT(settext(QString)));
    QObject::connect(this, SIGNAL(send_comm_params(QString, QString)), this->mintty_terminal,
                     SLOT(establish_comm(QString, QString)));
}

mintty::~mintty()
{
    delete ui;
}

void mintty::on_btn_connect_clicked()
{
    QString ipaddress_literal = this->ui->le_ipaddress->text();
    QString port_literal = this->ui->le_port->text();

    this->sendmessage("mintty v0.8, Copyright (C) 2015 AlphaGroup");
    this->send_comm_params(ipaddress_literal, port_literal);

    this->hide();
    mintty_terminal->setModal(true);
    mintty_terminal->exec();
}
