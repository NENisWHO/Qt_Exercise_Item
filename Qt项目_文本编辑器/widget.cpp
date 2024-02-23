#include "widget.h"
#include "ui_widget.h"
//#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QShortcut *shortOpen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),this);
    QShortcut *shortSave = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Save")),this);
    QShortcut *shortZoomIn = new QShortcut(QKeySequence(tr("Ctrl+=", "File|Save")),this);
    QShortcut *shortZoomOut = new QShortcut(QKeySequence(tr("Ctrl+-", "File|Save")),this);

    connect(shortOpen,&QShortcut::activated,[=](){
        on_btnOpen_clicked();
    });

    connect(shortSave,&QShortcut::activated,[=](){
        on_btnSave_clicked();
    });

    connect(shortZoomIn,&QShortcut::activated,[=](){
        zoomIn();
    });

    connect(shortZoomOut,&QShortcut::activated,[=](){
        zoomOut();
    });

    //虽然给你面一行代码进行了widget和ui的窗口关联，但是如果窗口大小变化时，里面的布局不会随之变化
    //通过下面一行的代码显示说明，窗口变化时布局及控件随之调整
    this->setLayout(ui->verticalLayout);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);//隐藏标题栏，和最大最小按钮
    //
    //connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onCurrentIndexChanged(int)));
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(onCursorPositionChanged()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::zoomIn()
{
    //获取textEdit字体信息
    QFont font = ui->textEdit->font();
    //获取当前字体的大小
    int fontSize = font.pointSize();
    if(fontSize == -1) return ;

    //改变字体大小，并设置字体大小
    int newFontSize = fontSize + 1;
    font.setPointSize(newFontSize);
    ui->textEdit->setFont(font);
}

void Widget::zoomOut()
{
    //获取textEdit字体信息
    QFont font = ui->textEdit->font();
    //获取当前字体的大小
    int fontSize = font.pointSize();
    if(fontSize == -1) return ;

    //改变字体大小，并设置字体大小
    int newFontSize = fontSize - 1;
    font.setPointSize(newFontSize);
    ui->textEdit->setFont(font);
}

//打开文件功能
void Widget::on_btnOpen_clicked()
{
    //打开
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                    "E:/QT_Example/QtLiangxu/37-notebook002",
                                                    tr("Text(*.txt)"));
    ui->textEdit->clear();  //在打开文本之前先清空ui界面的textEdit
    file.setFileName(fileName);     //打开的文件是fileName获取的路径文件
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug()<<"File open error.";
    }
    this->setWindowTitle(fileName + " -MyNoteBook");
    //读取
    QTextStream in(&file);
    //in.setCodec("UTF-8"); 不在手动输入
    //将ui->comboBox->currentText()转换为char
    QString str = ui->comboBox->currentText();
    const char *cstr = str.toStdString().c_str();//先转换成c++的string再转成char*
    //std::cout<<cstr;
    in.setCodec(cstr);//需要传进去char*的变量
    while(!in.atEnd()){
        QString context = in.readLine();    //一行一行读取
        ui->textEdit->append(context);
    }
    //关闭，，，以增加了关闭功能
    //file.close();
}

//保存文件功能
void Widget::on_btnSave_clicked()
{
    if(!file.isOpen()){
        //打开文件
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                        "E:/QT_Example/QtLiangxu/37-notebook002",
                                                        tr("Text(*.txt)"));
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
            qDebug()<<"File open error.";
        }
        this->setWindowTitle(fileName + " -MyNoteBook");    //这里是实现窗口标题的改变，知道打开的是那个文件

    }
    //写入文件
    QTextStream out(&file);
    QString str = ui->comboBox->currentText();
    const char *cstr = str.toStdString().c_str();//先转换成c++的string再转成char*
    out.setCodec(cstr);
    //out.setCodec("ANSI");

    QString content = ui->textEdit->toPlainText();//将ui的textEdit的文本
    out<<content;
    //关闭文件，，，已增加了关闭功能
    //file.close();
}
//关闭文件功能
void Widget::on_btnClose_clicked()
{
    QMessageBox msgBox;
    int ret = QMessageBox::warning(this, tr("MyNoteBook Close"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel,
                                   QMessageBox::Save);
    // 这里是点击关闭按钮后弹出详细框提供关闭选择
    switch (ret) {
      case QMessageBox::Save:
          // Save was clicked
          on_btnSave_clicked();
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
        ui->textEdit->clear();//清空textEdit
        if(file.isOpen()){
            file.close();
            this->setWindowTitle(" -MyNoteBook");
        }
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

//void Widget::onCurrentIndextChanged(int index)
//{
//    //切换编码后，重新读取
//    qDebug()<<ui->comboBox->currentIndex();
//    ui->textEdit->clear();
//    if(file.isOpen()){
//        QTextStream in(&file);
//        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
//        //文件的光标回到头QFile的seek
//        file.seek(0);
//        while(!in.atEnd()){
//            QString context = in.readLine();
//            ui->textEdit->append(context);
//        }
//    }
//}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    //切换编码后，重新读取
    //qDebug()<<index;

    ui->textEdit->clear();  //不管打开还是没打开文件，当comboBox改变时，先清空文本
    if(file.isOpen()){      //打开文件以comboBox现在的编码方式读取文本
        QTextStream in(&file);
        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
        //因为在打开文件时光标已经在尾部了，要让光标回到头部才能正常运行下面的代码
        //文件的光标回到头QFile的seek
        file.seek(0);
        while(!in.atEnd()){
            QString context = in.readLine();
            ui->textEdit->append(context);
        }
    }
}
//实现光标位置更新的功能
void Widget::onCursorPositionChanged()
{

    QTextCursor cursor=ui->textEdit->textCursor();
    //qDebug()<<cursor.columnNumber() + 1<<" ;"<<cursor.blockNumber() + 1;//lie hang
    QString blockNum = QString::number(cursor.blockNumber());   //number()将整形转换为QString型
    QString columnNum = QString::number(cursor.columnNumber());
    const QString labelMes = "L:"+blockNum+"   C:"+columnNum;
    ui->labelPosition->setText(labelMes);

    //设置当前行高亮
//    struct ExtraSelection
//    {
//        QTextCursor cursor;
//        QTextCharFormat format;
//    };
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection ext;

    //1.知道当前行
    ext.cursor = ui->textEdit->textCursor();

    QBrush qBrush(Qt::yellow);
    //2.颜色
    ext.format.setBackground(qBrush);
    //配置属性，整行显示，没有这句话不行
    ext.format.setProperty(QTextFormat::FullWidthSelection, true);
    //3.设置
    //把ext加到容器里
    extraSelections.append(ext);
    ui->textEdit->setExtraSelections(extraSelections);
}
