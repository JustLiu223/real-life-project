#include "mytcpsocket.h"
#include <QDebug>
#include "protocol.h"
#include "mytcpserver.h"
MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));             //读取套接字数据
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffline()));              //客户端关闭时调用
}

QString MyTcpSocket::getName()
{
    return m_strName;
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
    case ENUM_MSG_TYPE_REGIST_REQUEST:               //接收到客户端注册请求
    {
        char caName[32] = {'\0'};                      //获取用户名密码
        char caPwd[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        strncpy(caPwd,pdu->caData+32,32);
        bool ret = OpeDB::getInstance().handleRegist(caName,caPwd);  //对数据库进行操作
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;           //返回给客户端注册响应

        if (ret){
                strcpy(respdu->caData,REGIST_OK);    //注册成功
        }
        else{
                strcpy(respdu->caData,REGIST_FAILED); //注册失败
        }
        write((char*)respdu,respdu->uiPDULen);   //发送给客户端
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:                   //客户端登录请求
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        strncpy(caPwd,pdu->caData+32,32);
        bool ret = OpeDB::getInstance().handleLogin(caName,caPwd);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;          //客户端登录请求响应

        if (ret){
            strcpy(respdu->caData,LOGIN_OK);          //登录成功
            m_strName = caName;                    //保存登录用户名
        }
        else{
            strcpy(respdu->caData,LOGIN_FAILED);    //登录失败
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:                     //客户端查看在线用户请求
    {
        QStringList ret = OpeDB::getInstance().handleAllOnline();   //操作数据库
        uint uiMsgLen = ret.size()*32;
        PDU *respdu = mkPDU(uiMsgLen);
        respdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;   //客户端在线用户响应
        for(int i =0;i<ret.size();i++){                               //遍历读取的在线用户名
            memcpy((char*)respdu->caMsg+i*32
                   ,ret.at(i).toStdString().c_str()
                   ,ret.at(i).size());
        }
        write((char*)respdu,respdu->uiPDULen);             //发送给客户端
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USR_REQUEST:            //客户端查找用户请求
    {
        int ret = OpeDB::getInstance().handleSearchUsr(pdu->caData);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_RESPOND;       //客户端查找用户响应
        if(ret == -1){
            strcpy(respdu->caData,USR_NO);                   //用户不存在
        }
        else if(ret == 1){
            strcpy(respdu->caData,USR_ONLINE);             //用户存在且在线
        }
        else if(ret ==0)
        {
            strcpy(respdu->caData,USR_OFFLINE);         //用户存在但不在线
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        char caPerName[32] = {'\0'};
        char caName[32] = {'\0'};
        strncpy(caPerName,pdu->caData,32);
        strncpy(caName,pdu->caData+32,32);
        int ret = OpeDB::getInstance().handleADDFriend(caPerName,caName);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
        if(-1 == ret){
            strcpy(respdu->caData,UNKNOW_ERROR);
        }
        else if(0 == ret){
            strcpy(respdu->caData,USR_EXISTED);
        }
        else if(1 == ret){
            MyTcpServer::getInstance().resend(caPerName,pdu);
        }
        else if(2 == ret){
            strcpy(respdu->caData,USR_OFFLINE);
        }
        else if(3 == ret){
            strcpy(respdu->caData,USR_NO);
        }

        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
    {
        char caName[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        QStringList ret =  OpeDB::getInstance().handleFlushFriend(caName);
        uint uiMsgLen = ret.size()*32;
        PDU *respdu = mkPDU(uiMsgLen);
        respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
        for (int i=0;i<ret.size();i++){
            memcpy((char*)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(),ret.at(i).size());
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        char caName[32] = {'\0'};
        char caFriendName[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        strncpy(caFriendName,pdu->caData+32,32);
        OpeDB::getInstance().handleDelFriend(caName,caFriendName);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
        strcpy(respdu->caData,DEL_FRIEND_OK);
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        MyTcpServer::getInstance().resend(caFriendName,pdu);
        break;

    }
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        char caPerName[32] = {'\0'};
        memcpy(caPerName,pdu->caData+32,32);
        MyTcpServer::getInstance().resend(caPerName,pdu);
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;

    // qDebug()<<caName<<caPwd<<pdu->uiMsgType;
}

void MyTcpSocket::clientOffline()                      //客户端关闭
{
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());                //获取用户名
    emit offline(this);
}
