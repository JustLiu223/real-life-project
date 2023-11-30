#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"
#include <Qfile>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

TcpServer::TcpServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();                 //加载配置
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);    //监听客户端的连接
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");  //文件路径
    if( file.open(QIODevice::ReadOnly))
    {
        QByteArray baData =  file.readAll();
        QString strData = baData.toStdString().c_str();  //读取转为字符串
        // qDebug() << strData;
        file.close();
        strData.replace("\r\n"," "); //替换多余字符
        // qDebug() << strData;
        QStringList strList =  strData.split(" "); //空格分割
        // for(int i = 0;i<strList.size();i++)
        // {
        //     qDebug() << strList[i];
        // }
        m_strIP = strList.at(0);  //第一行为ip
        m_usPort = strList.at(1).toUShort(); //第二行为 port
        qDebug() << "ip:" << m_strIP<<"port:"<<m_usPort;
    }
    else
    {
        QMessageBox::critical(this,"open config","open config failed"); //读取失败提示
    }
}
