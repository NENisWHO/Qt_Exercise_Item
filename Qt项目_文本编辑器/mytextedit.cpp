#include "mytextedit.h"

#include <QWheelEvent>
#include <QDebug>

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void MyTextEdit::wheelEvent(QWheelEvent *e)
{
    //��һ����Ҫ��ui�����textEdit����ΪMyTextEdit
    //qDebug()<<e->angleDelta().y();
    if(ctrlkeyPressed == 1){
        if(e->angleDelta().y() > 0){
            zoomIn();
        }else if(e->angleDelta().y() < 0){
            zoomOut();
        }
        e->accept();//�¼��������
    }else{
        //���ctrlû���¾Ͳ�ִ�зŴ���С�������չ��ֵ�֮ǰ�Ĺ�������
        QTextEdit::wheelEvent(e);
    }

}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    //��ctrl���Ʊ��� �������Ź��ֻ����仯
    if(e->key() == Qt::Key_Control){
        ctrlkeyPressed = 1; //ctrl����
    }
    QTextEdit::keyPressEvent(e);
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        ctrlkeyPressed = 0; //ctrl�ɿ�
    }
    QTextEdit::keyReleaseEvent(e);
}
