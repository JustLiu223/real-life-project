#include "friend.h"
#include "protocol.h"
#include"tcpclient.h"
#include<QInputDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <privatechat.h>
Friend ::Friend(QWidget *parent)
    : QWidget{parent}
{
    m_pOnline = new Online;                                                                      //在线
    m_pShowMsgTE = new QTextEdit;
    m_pFriendListWiget = new QTableWidget;
    m_pInputMsgLE = new QLineEdit;
    m_pDelFriendPB= new QPushButton("删除好友");
    m_pFlashFriendPB = new QPushButton("刷新好友");
    m_pShowOnlineUsrPB= new QPushButton("显示在线");
    m_pSearchUsrPB= new QPushButton("查找用户");
    m_pMsgSendPB= new QPushButton("发送");
    m_pPrivateChatPB= new QPushButton("私聊");


    // m_pFriendListWiget->setHorizontalHeaderLabels()
    m_pFriendListWiget->setColumnCount(2);
    // m_pFriendListWiget->setRowCount(3);
    QStringList headerLabels;
    headerLabels = {"用户名","状态"} ;
    m_pFriendListWiget->setHorizontalHeaderLabels(headerLabels);
    m_pFriendListWiget->resizeColumnsToContents();

    QVBoxLayout *pRightPBVBL = new QVBoxLayout;          //垂直布局
    pRightPBVBL->addWidget(m_pDelFriendPB);
    pRightPBVBL->addWidget(m_pFlashFriendPB);
    pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
    pRightPBVBL->addWidget(m_pSearchUsrPB);
    pRightPBVBL->addWidget(m_pPrivateChatPB);
    QHBoxLayout *pTopHBL = new QHBoxLayout;               //水平布局
    pTopHBL->addWidget(m_pShowMsgTE);
    pTopHBL->addWidget(m_pFriendListWiget);
    pTopHBL->addLayout(pRightPBVBL);

    QHBoxLayout * pMsgHBL = new QHBoxLayout;
    pMsgHBL->addWidget(m_pInputMsgLE);
    pMsgHBL->addWidget(m_pMsgSendPB);


    QVBoxLayout *pMain = new QVBoxLayout;                    //垂直布局
    pMain->addLayout(pTopHBL);
    pMain->addLayout(pMsgHBL);
    pMain->addWidget(m_pOnline);                                 //加载在线显示
    m_pOnline->hide();              //开始时隐藏
    setLayout(pMain);
    connect(m_pShowOnlineUsrPB,SIGNAL(clicked(bool)),this,SLOT(showOnline()));
    connect(m_pSearchUsrPB,SIGNAL(clicked(bool)),this,SLOT(searchUsr()));
    connect(m_pFlashFriendPB,SIGNAL(clicked(bool)),this,SLOT(flushFriend()));
    connect(m_pDelFriendPB,SIGNAL(clicked(bool)),this,SLOT(deleteFriend()));
    connect(m_pPrivateChatPB,SIGNAL(clicked(bool)),this,SLOT(privateChat()));
}

void Friend::showAllOnlineUsr(PDU *pdu)             //显示在线用户
{
    if(NULL==pdu){                       //数据检查
        return;
    }
    m_pOnline->showUsr(pdu);

}

void Friend::updateFriendList(PDU *pdu)
{
    if(NULL==pdu){                       //数据检查
        return;
    }
    m_pFriendListWiget->setRowCount(0);
    uint uiSize = pdu->uiMsgLen/32;
    // uint dataSize = sizeof(pdu->uiMsgLen);
    char tmp[32] = {'\0'};
    QStringList FriendNames;
    QStringList Ids;
    // for(uint i=0;i<uiSize;i++){
    //     memcpy(caName,(char*)(pdu->caMsg)+i*32,32);
    //     int row = m_pFriendListWiget->rowCount();
    //     m_pFriendListWiget->insertRow(row);
    //     m_pFriendListWiget->setItem(row,0,new QTableWidgetItem(caName));
    //     m_pFriendListWiget->setItem(row,1,new QTableWidgetItem("在线"));
    // }
    for (uint i=0;i<uiSize;i+=2){
        if(i+1<uiSize){
            memcpy(tmp,(char*)(pdu->caMsg)+i*32,32);
            FriendNames.append(tmp);

            memcpy(tmp,(char*)(pdu->caMsg)+(i+1)*32,32);
            Ids.append(tmp);
        }


    }
    for (uint i=0;i<FriendNames.size();i++){
            int row = m_pFriendListWiget->rowCount();
            m_pFriendListWiget->insertRow(row);
            m_pFriendListWiget->setItem(row,0,new QTableWidgetItem(FriendNames[i]));
            if(Ids[i]=='1'){
            m_pFriendListWiget->setItem(row,1,new QTableWidgetItem("在线"));
            }
            else{
                m_pFriendListWiget->setItem(row,1,new QTableWidgetItem("离线"));
            }
    }
    // qDebug() << "Names:" << FriendNames;
    // qDebug() << "IDs:" << Ids;

    // free(pdu);
    // pdu=NULL;
}

void Friend::showOnline()                 //显示在线用户
{
    if(m_pOnline->isHidden()){
        m_pOnline->show();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);            //调用m_tcpSocket发送显示在线请求
        free(pdu);
        pdu = NULL;
    }
    else{
        m_pOnline->hide();
    }
}

void Friend::searchUsr()                                  //搜索用户
{
     m_strSearchName = QInputDialog::getText(this,"搜索","用户名:");
    if(!m_strSearchName.isEmpty()){
        // qDebug()<<name;
        PDU *pdu = mkPDU(0);
        memcpy(pdu->caData,m_strSearchName.toStdString().c_str(),m_strSearchName.size());
        pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_REQUEST;                      //发送搜索用户请求
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void Friend::flushFriend()
{
    QString strName = TcpClient::getInstance().loginName();
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
}

void Friend::deleteFriend()
{
    if(m_pFriendListWiget->currentItem() !=NULL){
        QString strFriendName =  m_pFriendListWiget->currentItem()->text();
        PDU *pdu = mkPDU(0);
        pdu ->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        QString strSelfName = TcpClient::getInstance().loginName();
        memcpy(pdu->caData,strSelfName.toStdString().c_str(),strSelfName.size());
        memcpy(pdu->caData+32,strFriendName.toStdString().c_str(),strFriendName.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
}

void Friend::privateChat()
{
    if(m_pFriendListWiget->currentItem() !=NULL){
        QString strChatName =  m_pFriendListWiget->currentItem()->text();
        PrivateChat::getInstance().setChatName(strChatName);
        if(PrivateChat::getInstance().isHidden()){
            PrivateChat::getInstance().show();
        }
    }
}
