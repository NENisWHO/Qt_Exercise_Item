#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout_3);
}

Widget::~Widget()
{
    delete ui;
}

//获取本机主机名和IP地址按钮
void Widget::on_btnGetHostInfo_clicked()
{
    ui->plainTextEdit->clear();
    QString hostName = QHostInfo::localHostName();  //本机主机名
    qDebug()<<hostName;
    ui->plainTextEdit->appendPlainText("HostName:"+hostName+"\n");

    QHostInfo hostInfo = QHostInfo::fromName(hostName); //本机IP地址
    QList<QHostAddress> addrList = hostInfo.addresses();//IP地址列表
    if(addrList.isEmpty()) return;
    foreach(QHostAddress host, addrList){
        bool show = ui->checkBox_OnlyIPV4->isChecked(); //只显示ipv4
        //Returns the network layer protocol of the host address.
        show = show ? (host.protocol() == QAbstractSocket::IPv4Protocol):true;
        if(show){
            ui->plainTextEdit->appendPlainText("protool:"+protocolName(host.protocol()));//协议
            ui->plainTextEdit->appendPlainText("HostIPAdddress:"+host.toString());
            ui->plainTextEdit->appendPlainText(QString("isGolbal() = %1\n").arg(host.isGlobal()));
        }
    }
}

//通过协议类型返回协议名称字符串
QString Widget::protocolName(QAbstractSocket::NetworkLayerProtocol protocol)
{
    switch (protocol) {
    case QAbstractSocket::IPv4Protocol:
        return "IPV4";
    case QAbstractSocket::IPv6Protocol:
        return "IPV6";
    case QAbstractSocket::AnyIPProtocol:
        return "Any Internet Protocol";
    default:
        return "Unknown NetWork Layer Protocol";
    }
}

//根据枚举值返回字符串
QString Widget::interfaceType(QNetworkInterface::InterfaceType type)
{
    switch(type){
    case QNetworkInterface::Unknown:
        return "Unkown";
    case QNetworkInterface::Loopback:
        return "Loopback";
    case QNetworkInterface::Ethernet:
        return "Etherent";
    case QNetworkInterface::Wifi:
        return "Wifi";
    default:
        return "Other type";
    }
}

//查找域名的IP地址
void Widget::on_btnLookkup_clicked()
{
    ui->plainTextEdit->clear();
    QString hostName = ui->comboBox->currentText(); //读取主机名
    ui->plainTextEdit->appendPlainText("Searching for host information:"+hostName+"\n");
    QHostInfo::lookupHost(hostName, this,SLOT(do_lookedUpHostInfo(QHostInfo)));
}

//查找主机信息的槽函数
void Widget::do_lookedUpHostInfo(const QHostInfo &host)
{
    QList<QHostAddress> addrList = host.addresses();    //获取主机地址列表
    if(addrList.isEmpty())return;
    foreach(QHostAddress host, addrList)
    {
        bool show = ui->checkBox_OnlyIPV4->isChecked(); //只显示ipv4
        //Returns the network layer protocol of the host address.
        show = show ? (host.protocol() == QAbstractSocket::IPv4Protocol):true;
        if(show){
            ui->plainTextEdit->appendPlainText("protool:"+protocolName(host.protocol()));//协议
            ui->plainTextEdit->appendPlainText(host.toString());
            ui->plainTextEdit->appendPlainText(QString("isGolbal() = %1\n").arg(host.isGlobal()));
        }
    }
}

//allInterfaces()
void Widget::on_btnAllInterface_2_clicked()
{
    ui->plainTextEdit->clear();
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();//网络接口列表
    foreach(QNetworkInterface interface,list){
        if(!interface.isValid())
            continue;
//        The name of the device
//        Hardware devices
//        Interface type
//        Subnet mask
//        broadcast address
        ui->plainTextEdit->appendPlainText("The name of the device:"+interface.humanReadableName());
        ui->plainTextEdit->appendPlainText("Hardware devices:"+interface.hardwareAddress());
        ui->plainTextEdit->appendPlainText("Interface type:"+interfaceType(interface.type()));
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();//地址列表
        foreach(QNetworkAddressEntry entry, entryList){
            ui->plainTextEdit->appendPlainText("    IP Address:"+entry.ip().toString());
            ui->plainTextEdit->appendPlainText("    Subnet mask:"+entry.netmask().toString());
            ui->plainTextEdit->appendPlainText("    broadcast address:"+entry.broadcast().toString()+"\n");
        }
    }
}

//alladdress
void Widget::on_btnAllAddress_clicked()
{
    ui->plainTextEdit->clear();
    QList<QHostAddress> addrelist = QNetworkInterface::allAddresses();//IP地址列表
    if(addrelist.isEmpty()) return;
    foreach(QHostAddress host, addrelist){
        bool show = ui->checkBox_OnlyIPV4->isChecked();
        show = show ? (host.protocol() == QAbstractSocket::IPv4Protocol):true;
        if(show){
            ui->plainTextEdit->appendPlainText("protocol:"+protocolName(host.protocol()));
            ui->plainTextEdit->appendPlainText("IP Address:"+host.toString());
            ui->plainTextEdit->appendPlainText(QString("isGrobal() = %1\n").arg(host.isGlobal()));
        }
    }
}
