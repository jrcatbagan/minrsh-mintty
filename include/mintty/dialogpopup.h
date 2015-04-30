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
    void on_lineEdit_editingFinished();

private:
    Ui::DialogPopup *ui;
};

#endif // DIALOGPOPUP_H
