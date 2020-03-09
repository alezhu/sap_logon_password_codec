#include "DialogList.h"
#include "ui_DialogList.h"

DialogList::DialogList(QWidget *parent) :
    QDialog(parent, Qt::WindowMinMaxButtonsHint),
    ui(new Ui::DialogList)
{
    ui->setupUi(this);
}

DialogList::~DialogList()
{
    delete ui;
}

void DialogList::setData(const QStringList &data)
{
    ui->edList->setText(data.join("\r\n"));
}

void DialogList::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogList::on_btnOK_accepted()
{
    this->close();
}
