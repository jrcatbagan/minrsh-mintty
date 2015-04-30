#include "putty.h"
#include "dialogpopup.h"
#include "ui_putty.h"

Putty::Putty(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Putty)
{
    ui->setupUi(this);
}

Putty::~Putty()
{
    delete ui;
}

// Load button
void Putty::on_pushButton_Load_clicked()
{

}

// Save button
void Putty::on_pushButton_Save_clicked()
{

}

// Delete button
void Putty::on_pushButton__Delete_clicked()
{

}

// Connect button-------------------------------
void Putty::on_pushButton_Connect_clicked()
{
    // popup window
    DialogPopup dialpop;
    dialpop.setModal(true);
    dialpop.exec();
}

// Cancel button
void Putty::on_pushButton_Cancel_clicked()
{

}

// Raw radio button
void Putty::on_radioButton_Raw_clicked()
{

}

// Rlogin radio button
void Putty::on_radioButton_Rlogin_clicked()
{

}

// SSH radio button
void Putty::on_radioButton_SSH_clicked()
{

}

// Always radio button
void Putty::on_radioButton_Always_clicked()
{

}

// Never radio button
void Putty::on_radioButton_Never_clicked()
{

}

// Only radio button
void Putty::on_radioButton_Only_clicked()
{

}

// IP address text box
void Putty::on_lineEdit_IPin_textEdited(const QString &arg1)
{

}

// Port text box
void Putty::on_lineEdit_Portin_textEdited(const QString &arg1)
{

}

// Server text box
void Putty::on_lineEdit_Serverline_textEdited(const QString &arg1)
{

}

// Setting text box
void Putty::on_lineEdit_Setting_textEdited(const QString &arg1)
{

}
