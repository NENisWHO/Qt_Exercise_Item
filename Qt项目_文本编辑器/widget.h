#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QComboBox>
#include <QMessageBox>
#include <QShortcut>
#include "mytextedit.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void zoomIn();
    void zoomOut();
private slots:
    void on_btnOpen_clicked();

    void on_btnSave_clicked();

    void on_btnClose_clicked();

    //void onCurrentIndextChanged(int index);//ѡ����뷽ʽ�Ĳۺ���

    void on_comboBox_currentIndexChanged(int index);

    //
    void onCursorPositionChanged();


private:
    Ui::Widget *ui;
private:
    QFile file;//�ļ�����
};

#endif // WIDGET_H
