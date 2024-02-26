#include "widget.h"
#include "ui_widget.h"

#include <QDir>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//clearButtonEnabled : bool
//This property holds whether the line edit displays a clear button when it is not empty.
//If enabled, the line edit displays a trailing clear button when it contains some text,
//            otherwise the line edit does not show a clear button (the default).
    ui->editURL->setClearButtonEnabled(true);
    this->setLayout(ui->verticalLayout);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::do_finished()
{//网络响应结束
    QFileInfo fileInfo(downloadedFile->fileName()); //获取下载文件的文件名
    downloadedFile->close();
    delete downloadedFile;  //删除临时文件
    reply->deleteLater();   //由主事件循环删除此文件对象
    if(ui->checkBoxOpen->isChecked()){
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    }
    ui->btnDownload->setEnabled(true);
}

void Widget::do_readyRead()
{
    //读取下载数据
    downloadedFile->write(reply->readAll());
//    qint64 QIODevice::write(const char *data, qint64 maxSize)
//    Writes at most maxSize bytes of data from data to the device.
//            Returns the number of bytes that were actually written,
//            or -1 if an error occurred.
//    QByteArray QIODevice::readAll()
//    Reads all remaining data from the device, and returns it as a byte array.

}

void Widget::do_downloadProgress(qint64 bytesRead, qint64 totalBytes)
{//下载进度
    ui->progressBar->setMaximum(totalBytes);
    //Holds the total amount of data to download in bytes.
    ui->progressBar->setValue(bytesRead);
}


//默认了路径 按钮
void Widget::on_btnDefaultPath_clicked()
{
//    [static] QString QDir::currentPath()
//    Returns the absolute path of the application's current directory.
//            The current directory is the last directory set with QDir::setCurrent() or,
//            if that was never called,
//            the directory at which this application was started at by the parent process.
    QString curPath = QDir::currentPath();  //返回当前绝对路径给QString变量
    QDir dir(curPath);                      //定义QDir变量
    QString sub = "temp";
//    bool QDir::mkdir(const QString &dirName) const
//    Creates a sub-directory called dirName.
    dir.mkdir(sub);                         //当前路径下创立名为sub的文件夹
    ui->editPath->setText(curPath+"/"+sub+"/"); //设置lineedit的文字
}

//下载按钮，开始下载
void Widget::on_btnDownload_clicked()
{
//    QString QString::trimmed() const
//    Returns a string that has whitespace removed from the start and the end.
    QString urlSpec = ui->editURL->text().trimmed();    //将editURL中的文字给QString变量赋值
    //urlspec判空
    if(urlSpec.isEmpty()){
        QMessageBox::information(this,"error","Please specify the download address");
        return;
    }
//    [static] QUrl QUrl::fromUserInput(const QString &userInput)
//    Returns a valid URL from a user supplied userInput string if one can be deducted.
//    In the case that is not possible, an invalid QUrl() is returned.
    QUrl newUrl = QUrl::fromUserInput(urlSpec); //从urlSpaec里获取url
    //newurl判有效
    if(!newUrl.isValid()){
//        QString QUrl::errorString() const
//        Returns an error message if the last operation that modified this QUrl object ran into a parsing error.
//                If no error was detected,
//                this function returns an empty string and isValid() returns true.
        QString info = "The URL is invalid:"+urlSpec+"\n error:"+newUrl.errorString();
        QMessageBox::information(this,"error",info);
        return;
    }

    QString tempDir = ui->editPath->text().trimmed();   //临时目录
    if(tempDir.isEmpty()){
        QMessageBox::information(this,"error","Please specify the download address");
        return;
    }

    QString fullFileName = tempDir + newUrl.fileName(); //文件名
//    QString QUrl::fileName(QUrl::ComponentFormattingOptions options = FullyDecoded) const
//    Returns the name of the file, excluding the directory path.
    if(QFile::exists(fullFileName)){ //如果文件存在
        QFile::remove(fullFileName);    //删除文件
//        bool QFile::remove()
//        Removes the file specified by fileName(). Returns true if successful; otherwise returns false.
//        The file is closed before it is removed.
    }

    downloadedFile = new QFile(fullFileName);   //创建临时文件
    if(!downloadedFile->open(QIODevice::WriteOnly)){
        QMessageBox::information(this,"error","Temporary file open error");
    }
    ui->btnDownload->setEnabled(false);
    //发送网络请求，创建网络相应
    reply = networkManager.get(QNetworkRequest(newUrl));    //网络响应从网络管理里面获取网络请求
//    QNetworkReply *QNetworkAccessManager::get(const QNetworkRequest &request)
//            Posts a request to obtain the contents of the target request and
//            returns a new QNetworkReply object opened for reading
//            which emits the readyRead() signal whenever new data arrives.
//            The contents as well as associated headers will be downloaded.
//    QObject *QObject::parent() const
//    Returns a pointer to the parent object.

    connect(reply, SIGNAL(readyRead()), this, SLOT(do_readyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(do_downloadProgress(qint64,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(do_finished()));
//    [signal] void QIODevice::readyRead()
//    This signal is emitted once every time new data is available for
//    reading from the device's current read channel.
//    It will only be emitted again once new data is available,
//    such as when a new payload of network data has arrived on your network socket,
//    or when a new block of data has been appended to your device.
//    [signal] void QNetworkReply::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
//    This signal is emitted to indicate the progress of the download part of
//    this network request, if there's any. If there's no download associated with this request,
//    this signal will be emitted once with 0 as the value of both bytesReceived and bytesTotal.
//    [signal] void QNetworkReply::finished()
//    This signal is emitted when the reply has finished processing.
//    After this signal is emitted, there will be no more updates to the reply's data or metadata.
}

void Widget::on_editURL_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
}


