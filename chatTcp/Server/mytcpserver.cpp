#include "mytcpserver.h"
#include <QList>
MyTcpServer::MyTcpServer()
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new client connected";                  //提示有新的连接
    MyTcpSocket * pTcpSocket = new MyTcpSocket;
    pTcpSocket ->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
}
