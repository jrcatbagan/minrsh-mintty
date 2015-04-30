#ifndef PUTTY_H
#define PUTTY_H

#include <QMainWindow>

namespace Ui {
class Putty;
}

class Putty : public QMainWindow
{
    Q_OBJECT

public:
    explicit Putty(QWidget *parent = 0);
    ~Putty();

private slots:
    void on_pushButton_Load_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton__Delete_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_Cancel_clicked();

    void on_radioButton_Raw_clicked();

    void on_radioButton_Rlogin_clicked();

    void on_radioButton_SSH_clicked();

    void on_radioButton_Always_clicked();

    void on_radioButton__Never_clicked();

    void on_radioButton_Only_clicked();

    void on_lineEdit_IPin_textEdited(const QString &arg1);

    void on_lineEdit_Portin_textEdited(const QString &arg1);

    void on_lineEdit_Serverline_textEdited(const QString &arg1);

    void on_lineEdit_Setting_textEdited(const QString &arg1);

private:
    Ui::Putty *ui;
};

#endif // PUTTY_H
