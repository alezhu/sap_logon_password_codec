#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_btEncrypt_clicked();

    void on_btDecrypt_clicked();

    void on_btFileOpenDlg_clicked();

    void on_btDecrypt_2_clicked();

private:
    Ui::MainWindow *ui;
    bool bAboutAdded = false;

    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
};
