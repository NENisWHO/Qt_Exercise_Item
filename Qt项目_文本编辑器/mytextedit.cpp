#include "mytextedit.h"

#include <QWheelEvent>
#include <QDebug>

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void MyTextEdit::wheelEvent(QWheelEvent *e)
{
    //这一步还要将ui界面的textEdit提升为MyTextEdit
    //qDebug()<<e->angleDelta().y();
    if(ctrlkeyPressed == 1){
        if(e->angleDelta().y() > 0){
            zoomIn();
        }else if(e->angleDelta().y() < 0){
            zoomOut();
        }
        e->accept();//事件处理完毕
    }else{
        //如果ctrl没按下就不执行放大缩小，，按照滚轮的之前的功能运行
        QTextEdit::wheelEvent(e);
    }

}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    //用ctrl控制避免 字体随着滚轮滑动变化
    if(e->key() == Qt::Key_Control){
        ctrlkeyPressed = 1; //ctrl按下
    }
    QTextEdit::keyPressEvent(e);
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        ctrlkeyPressed = 0; //ctrl松开
    }
    QTextEdit::keyReleaseEvent(e);
}
