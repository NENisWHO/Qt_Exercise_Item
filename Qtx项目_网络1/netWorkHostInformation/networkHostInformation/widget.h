#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnGetHostInfo_clicked();
    void on_btnLookkup_clicked();
    void do_lookedUpHostInfo(const QHostInfo &host);

    void on_btnAllInterface_2_clicked();

    void on_btnAllAddress_clicked();

private:
    QString protocolName(QAbstractSocket::NetworkLayerProtocol protocol);
    QString interfaceType(QNetworkInterface::InterfaceType type);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
