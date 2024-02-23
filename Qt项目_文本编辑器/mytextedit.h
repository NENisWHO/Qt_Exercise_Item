#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>


class MyTextEdit : public QTextEdit
{
public:
    MyTextEdit(QWidget *parent);
protected:
    void wheelEvent(QWheelEvent *e) override;   //�����¼�
    void keyPressEvent(QKeyEvent *e) override;  //���̰����¼�
    void keyReleaseEvent(QKeyEvent *e) override;//����ʩ���¼�
private:
    bool ctrlkeyPressed = 0;    //ctrl������Ϊtrue���ͷ�Ϊfalse
};

#endif // MYTEXTEDIT_H
