#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "encoder.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include "qt_windows.h"
#include "shlwapi.h"
#include "SapShortCut.h"
#include "DialogList.h"
#include "DialogAbout.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto file = R"(%APPDATA%\SAP\Common\sapshortcut.ini)";
    ui->edFilePath->setText(file);

//    setFixedHeight(sizeHint().height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

static const char* prefix = "PW_";

QString encrypt(const QString &password)
{
    auto aPassword = password.toLocal8Bit();
    auto pBuffer = reinterpret_cast<uint8_t*>(aPassword.data());
    encode_decode( pBuffer,static_cast<size_t>(aPassword.size()));

    auto sEncrypted = prefix + QString::fromLocal8Bit(aPassword.toHex()).toUpper();
    return sEncrypted;
}

QString decrypt(const QString &password)
{
    auto aPassword = QByteArray::fromHex(password.toLatin1().replace(prefix,""));
    auto pBuffer = reinterpret_cast<uint8_t*>(aPassword.data());
    encode_decode( pBuffer,static_cast<size_t>(aPassword.size()));

    auto sDecrypted = QString(aPassword);
    return sDecrypted;
}

void MainWindow::on_btEncrypt_clicked()
{
    auto password = ui->edPassword->text().trimmed();
    if (!password.length()) return;


    ui->edEncrypted->setText(encrypt(password));

}

void MainWindow::on_btDecrypt_clicked()
{
    auto password = ui->edEncrypted->text().trimmed();
    if (!password.length()) return;


    ui->edPassword->setText(decrypt(password));
}


QString ExpandFilePath(const QString& filePath)
{
    wchar_t wcExpanded[MAX_PATH];
    auto swFilePath = filePath.toStdWString();
    ZeroMemory(wcExpanded,MAX_PATH);
    ExpandEnvironmentStrings(swFilePath.c_str(),wcExpanded,MAX_PATH);
    return QString::fromWCharArray(wcExpanded);
}

QString UnExpandFilePath(const QString& filePath)
{
    wchar_t wcExpanded[MAX_PATH];
    auto swFilePath = filePath.toStdWString();
    ZeroMemory(wcExpanded,MAX_PATH);
    PathUnExpandEnvStrings(swFilePath.c_str(),wcExpanded,MAX_PATH);
    return QString::fromWCharArray(wcExpanded);
}

void MainWindow::on_btFileOpenDlg_clicked()
{
    auto sFilePath = ExpandFilePath(this->ui->edFilePath->text().trimmed());
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::FileMode::ExistingFile);
    QFileInfo fileInfo(sFilePath);
    dlg.selectFile(fileInfo.fileName());
    dlg.setDirectory(fileInfo.dir());
    if( QDialog::DialogCode::Accepted == dlg.exec()){
        ui->edFilePath->setText(UnExpandFilePath(dlg.selectedFiles().first().replace("/","\\")));
    }

}

void MainWindow::on_btDecrypt_2_clicked()
{
    auto sFilePath = ExpandFilePath(this->ui->edFilePath->text().trimmed());
    if(!QFile::exists(sFilePath)) {
        QMessageBox::critical(this,"","File not found");
        return;
    }

    SapShortCut oSapShortCut;
    oSapShortCut.load(sFilePath.toStdWString());
    auto encryptedData = oSapShortCut.getData();

    QStringList listData;

    std::wstring eq(L" = ");
    for (auto&& [key, value] : encryptedData) {
        auto sDecrypted = decrypt(QString::fromStdWString(value));
        listData.append(QString::fromStdWString(key) + " = " + sDecrypted);
    }


    auto dlgList = std::make_unique<DialogList>(this);
    dlgList->setModal(true);
    dlgList->setData(listData);
    dlgList->exec();

}

enum WM_USER_MSG:UINT_PTR {
    ABOUT
};

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = static_cast<MSG*>(message);
    if (!bAboutAdded) {
        auto hMenu = GetSystemMenu(msg->hwnd, false);

        if(hMenu) {
            AppendMenu(hMenu, MF_STRING, WM_USER_MSG::ABOUT, L"About");
            bAboutAdded = true;
        }
    }

    switch (msg->message) {
        case WM_SYSCOMMAND:
            if (WM_USER_MSG::ABOUT == LOWORD(msg->wParam))
            {
                auto dlgAbout = new DialogAbout(this);
                dlgAbout->exec();
                delete dlgAbout;
            }
            break;
    }

    return QMainWindow::nativeEvent(eventType, message, result);
}
