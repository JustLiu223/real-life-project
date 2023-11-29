#include "mytcpsocket.h"
#include <QDebug>
#include "protocol.h"
MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));
}

void MyTcpSocket::recvMsg()
{
    qDebug() << this->bytesAvailable();  //当前可读数据大小
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));  //收数据
    // qDebug()<<pdu->uiMsgType<<(char*)pdu->caMsg;

    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        strncpy(caPwd,pdu->caData+32,32);
        bool ret = OpeDB::getInstance().handleRegist(caName,caPwd);  //handleRegist是否应为私有模式
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;

        if (ret){
                strcpy(respdu->caData,REGIST_OK);
        }
        else{
                strcpy(respdu->caData,REGIST_FAILED);
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;

    // qDebug()<<caName<<caPwd<<pdu->uiMsgType;
}
