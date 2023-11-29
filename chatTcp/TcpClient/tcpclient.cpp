#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <Qfile>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "protocol.h"
TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    resize(400,200);
    loadconfig();
    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnect()));
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));  //接收数据
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);

}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadconfig()
{
    QFile file(":/client.config");  //文件路径
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

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void TcpClient::recvMsg()
{
    qDebug() << m_tcpSocket.bytesAvailable();  //当前可读数据大小
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));  //收数据
    // qDebug()<<pdu->uiMsgType<<(char*)pdu->caMsg;

    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        if(0 == strcmp(pdu->caData,REGIST_OK)){
            QMessageBox::information(this,"注册",REGIST_OK);
        }
        else if(0 == strcmp(pdu->caData,REGIST_FAILED)){
              QMessageBox::information(this,"注册",REGIST_FAILED);
        }
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}

#if 0

void TcpClient::on_pushButton_clicked()  //发送按钮指令
{
    QString strMsg = ui -> lineEdit->text();  //读取编辑框内容
    if(!strMsg.isEmpty()){
        PDU *pdu = mkPDU(strMsg.size()+1);
        pdu ->uiMsgType = 8888;
        memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);  //发送数据
        free(pdu);
        pdu = NULL;
    }
    else{
        QMessageBox::warning(this,"信息发送","发送的信息不能为空");
    }
}

#endif

void TcpClient::on_login_pb_clicked()
{

}


void TcpClient::on_regist_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty()){
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;

    }
    else{
        QMessageBox::critical(this,"注册","注册失败：用户名或者密码为空！");
    }
}


void TcpClient::on_cancel_pb_clicked()
{

}

