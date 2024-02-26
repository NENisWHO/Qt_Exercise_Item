#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QFile>
#include <QMessageBox>
#include <QIODevice>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    QNetworkAccessManager networkManager;   //网络管理
    QNetworkReply *reply;                   //网络响应
    QFile *downloadedFile;                  //下载保存的临时文件
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void do_finished();
    void do_readyRead();
    void do_downloadProgress(qint64 bytesRead, qint64 totalBytes);
    void on_btnDownload_clicked();

    void on_btnDefaultPath_clicked();

    void on_editURL_textChanged(const QString &arg1);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
