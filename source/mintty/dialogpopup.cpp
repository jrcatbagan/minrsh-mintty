#include "dialogpopup.h"
#include "ui_dialogpopup.h"

DialogPopup::DialogPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPopup)
{
    ui->setupUi(this);
}

DialogPopup::~DialogPopup()
{
    delete ui;
}

// Popup window
void DialogPopup::on_textBrowser_textChanged()
{

}

void DialogPopup::on_lineEdit_returnPressed()
{
    QString pin = "";
    QString id = "";

    people *personType;

    id = lineEditID->text();
    pin = lineEditPin->();
}
