#ifndef DIALOGPOPUP_H
#define DIALOGPOPUP_H

#include <QDialog>

namespace Ui {
class DialogPopup;
}

class DialogPopup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPopup(QWidget *parent = 0);
    ~DialogPopup();

private slots:
    //void on_lineEdit_editingFinished();

    void on_textBrowser_textChanged();

    void on_lineEdit_returnPressed();

private:
    Ui::DialogPopup *ui;
};

#endif // DIALOGPOPUP_H
