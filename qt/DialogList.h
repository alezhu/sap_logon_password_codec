#pragma once

#include <QDialog>

namespace Ui {
class DialogList;
}

class DialogList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogList(QWidget *parent = nullptr);
    ~DialogList();

    void setData(const QStringList& data);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_btnOK_accepted();

private:
    Ui::DialogList *ui;

};

