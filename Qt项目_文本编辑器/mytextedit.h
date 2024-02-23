#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>


class MyTextEdit : public QTextEdit
{
public:
    MyTextEdit(QWidget *parent);
protected:
    void wheelEvent(QWheelEvent *e) override;   //滚轮事件
    void keyPressEvent(QKeyEvent *e) override;  //键盘按下事件
    void keyReleaseEvent(QKeyEvent *e) override;//键盘施放事件
private:
    bool ctrlkeyPressed = 0;    //ctrl键按下为true，释放为false
};

#endif // MYTEXTEDIT_H
