#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include "protocol.h"
#include "opedb.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    QString getName();
signals:
    void offline(MyTcpSocket *mysocket );
public slots:
    void recvMsg(); //接收消息
    void clientOffline();        //客户端关闭
private:
    QString m_strName;
};

#endif // MYTCPSOCKET_H
