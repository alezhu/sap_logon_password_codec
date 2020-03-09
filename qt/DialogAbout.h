#pragma once

#include <QDialog>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = nullptr);
    ~DialogAbout();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAbout *ui;
};

