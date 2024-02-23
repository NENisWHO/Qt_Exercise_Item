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

    //��Ȼ������һ�д��������widget��ui�Ĵ��ڹ���������������ڴ�С�仯ʱ������Ĳ��ֲ�����֮�仯
    //ͨ������һ�еĴ�����ʾ˵�������ڱ仯ʱ���ּ��ؼ���֮����
    this->setLayout(ui->verticalLayout);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);//���ر��������������С��ť
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
    //��ȡtextEdit������Ϣ
    QFont font = ui->textEdit->font();
    //��ȡ��ǰ����Ĵ�С
    int fontSize = font.pointSize();
    if(fontSize == -1) return ;

    //�ı������С�������������С
    int newFontSize = fontSize + 1;
    font.setPointSize(newFontSize);
    ui->textEdit->setFont(font);
}

void Widget::zoomOut()
{
    //��ȡtextEdit������Ϣ
    QFont font = ui->textEdit->font();
    //��ȡ��ǰ����Ĵ�С
    int fontSize = font.pointSize();
    if(fontSize == -1) return ;

    //�ı������С�������������С
    int newFontSize = fontSize - 1;
    font.setPointSize(newFontSize);
    ui->textEdit->setFont(font);
}

//���ļ�����
void Widget::on_btnOpen_clicked()
{
    //��
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                    "E:/QT_Example/QtLiangxu/37-notebook002",
                                                    tr("Text(*.txt)"));
    ui->textEdit->clear();  //�ڴ��ı�֮ǰ�����ui�����textEdit
    file.setFileName(fileName);     //�򿪵��ļ���fileName��ȡ��·���ļ�
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug()<<"File open error.";
    }
    this->setWindowTitle(fileName + " -MyNoteBook");
    //��ȡ
    QTextStream in(&file);
    //in.setCodec("UTF-8"); �����ֶ�����
    //��ui->comboBox->currentText()ת��Ϊchar
    QString str = ui->comboBox->currentText();
    const char *cstr = str.toStdString().c_str();//��ת����c++��string��ת��char*
    //std::cout<<cstr;
    in.setCodec(cstr);//��Ҫ����ȥchar*�ı���
    while(!in.atEnd()){
        QString context = in.readLine();    //һ��һ�ж�ȡ
        ui->textEdit->append(context);
    }
    //�رգ������������˹رչ���
    //file.close();
}

//�����ļ�����
void Widget::on_btnSave_clicked()
{
    if(!file.isOpen()){
        //���ļ�
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                        "E:/QT_Example/QtLiangxu/37-notebook002",
                                                        tr("Text(*.txt)"));
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
            qDebug()<<"File open error.";
        }
        this->setWindowTitle(fileName + " -MyNoteBook");    //������ʵ�ִ��ڱ���ĸı䣬֪���򿪵����Ǹ��ļ�

    }
    //д���ļ�
    QTextStream out(&file);
    QString str = ui->comboBox->currentText();
    const char *cstr = str.toStdString().c_str();//��ת����c++��string��ת��char*
    out.setCodec(cstr);
    //out.setCodec("ANSI");

    QString content = ui->textEdit->toPlainText();//��ui��textEdit���ı�
    out<<content;
    //�ر��ļ��������������˹رչ���
    //file.close();
}
//�ر��ļ�����
void Widget::on_btnClose_clicked()
{
    QMessageBox msgBox;
    int ret = QMessageBox::warning(this, tr("MyNoteBook Close"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel,
                                   QMessageBox::Save);
    // �����ǵ���رհ�ť�󵯳���ϸ���ṩ�ر�ѡ��
    switch (ret) {
      case QMessageBox::Save:
          // Save was clicked
          on_btnSave_clicked();
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
        ui->textEdit->clear();//���textEdit
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
//    //�л���������¶�ȡ
//    qDebug()<<ui->comboBox->currentIndex();
//    ui->textEdit->clear();
//    if(file.isOpen()){
//        QTextStream in(&file);
//        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
//        //�ļ��Ĺ��ص�ͷQFile��seek
//        file.seek(0);
//        while(!in.atEnd()){
//            QString context = in.readLine();
//            ui->textEdit->append(context);
//        }
//    }
//}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    //�л���������¶�ȡ
    //qDebug()<<index;

    ui->textEdit->clear();  //���ܴ򿪻���û���ļ�����comboBox�ı�ʱ��������ı�
    if(file.isOpen()){      //���ļ���comboBox���ڵı��뷽ʽ��ȡ�ı�
        QTextStream in(&file);
        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
        //��Ϊ�ڴ��ļ�ʱ����Ѿ���β���ˣ�Ҫ�ù��ص�ͷ������������������Ĵ���
        //�ļ��Ĺ��ص�ͷQFile��seek
        file.seek(0);
        while(!in.atEnd()){
            QString context = in.readLine();
            ui->textEdit->append(context);
        }
    }
}
//ʵ�ֹ��λ�ø��µĹ���
void Widget::onCursorPositionChanged()
{

    QTextCursor cursor=ui->textEdit->textCursor();
    //qDebug()<<cursor.columnNumber() + 1<<" ;"<<cursor.blockNumber() + 1;//lie hang
    QString blockNum = QString::number(cursor.blockNumber());   //number()������ת��ΪQString��
    QString columnNum = QString::number(cursor.columnNumber());
    const QString labelMes = "L:"+blockNum+"   C:"+columnNum;
    ui->labelPosition->setText(labelMes);

    //���õ�ǰ�и���
//    struct ExtraSelection
//    {
//        QTextCursor cursor;
//        QTextCharFormat format;
//    };
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection ext;

    //1.֪����ǰ��
    ext.cursor = ui->textEdit->textCursor();

    QBrush qBrush(Qt::yellow);
    //2.��ɫ
    ext.format.setBackground(qBrush);
    //�������ԣ�������ʾ��û����仰����
    ext.format.setProperty(QTextFormat::FullWidthSelection, true);
    //3.����
    //��ext�ӵ�������
    extraSelections.append(ext);
    ui->textEdit->setExtraSelections(extraSelections);
}
