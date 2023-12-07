#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>
#include "mytcpsocket.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();
    static MyTcpServer &getInstance();  //静态函数生成对象
    void incomingConnection(qintptr socketDescriptor);  //连接成功运行函数
    void resend(const char *pername,PDU *pdu);
public slots:
    void deleteSocket(MyTcpSocket *mytcpsocket);          //删除socket
private:
    QList<MyTcpSocket*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
