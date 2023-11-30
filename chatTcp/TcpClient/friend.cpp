#include "friend.h"
#include "protocol.h"
#include"tcpclient.h"
#include<QInputDialog>
Friend ::Friend(QWidget *parent)
    : QWidget{parent}
{
    m_pOnline = new Online;                                                                      //在线
    m_pShowMsgTE = new QTextEdit;
    m_pFriendListWidget = new QListWidget;
    m_pInputMsgLE = new QLineEdit;
    m_pDelFriendPB= new QPushButton("删除好友");
    m_pFlashFriendPB = new QPushButton("刷新好友");
    m_pShowOnlineUsrPB= new QPushButton("显示在线");
    m_pSearchUsrPB= new QPushButton("查找用户");
    m_pMsgSendPB= new QPushButton("发送");
    m_pPrivateChatPB= new QPushButton("私聊");
    QVBoxLayout *pRightPBVBL = new QVBoxLayout;          //垂直布局
    pRightPBVBL->addWidget(m_pDelFriendPB);
    pRightPBVBL->addWidget(m_pFlashFriendPB);
    pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
    pRightPBVBL->addWidget(m_pSearchUsrPB);
    pRightPBVBL->addWidget(m_pPrivateChatPB);
    QHBoxLayout *pTopHBL = new QHBoxLayout;               //水平布局
    pTopHBL->addWidget(m_pShowMsgTE);
    pTopHBL->addWidget(m_pFriendListWidget);
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
}

void Friend::showAllOnlineUsr(PDU *pdu)             //显示在线用户
{
    if(NULL==pdu){                       //数据检查
        return;
    }
    m_pOnline->showUsr(pdu);

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
