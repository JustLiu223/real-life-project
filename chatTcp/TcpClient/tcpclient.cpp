#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <Qfile>
#include <QByteArray> //动态字节数组。它特别适用于处理原始二进制数据和字符字符串。
#include <QString>       //字符串
#include <QDebug>        //调试输出
#include <QMessageBox>  //消息框
#include <QHostAddress>    //网络通信
#include "protocol.h"             //自定义协议
TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    resize(400,200);   //重定义大小，登录界面
    loadconfig();          //加载客户端配置
    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnect()));  //槽函数连接时调用
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));  //连接后调用接收数据
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);  //连接服务器

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

TcpClient &TcpClient::getInstance()   //单例模式构造对象
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void TcpClient::recvMsg()
{
    // qDebug() << m_tcpSocket.bytesAvailable();  //当前可读数据大小
    // 从网络中读取一个消息
    uint uiPDULen = 0;

    // 读取消息总长度（4字节），存储在uiPDULen中
    m_tcpSocket.read(reinterpret_cast<char*>(&uiPDULen), sizeof(uint));

    // 计算实际消息长度（减去PDU结构体的大小）
    uint uiMsgLen = uiPDULen - sizeof(PDU);

    // 创建PDU对象，用于存储消息数据
    PDU *pdu = mkPDU(uiMsgLen);

    // 从网络中读取剩余的消息数据，存储在pdu对象的内存中
    // 注意：这里假设mkPDU函数负责分配足够的内存给pdu对象
    m_tcpSocket.read(reinterpret_cast<char*>(pdu) + sizeof(uint), uiPDULen - sizeof(uint));
    // qDebug()<<pdu->uiMsgType<<(char*)pdu->caMsg;

    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:         //注册消息响应
    {
        if(0 == strcmp(pdu->caData,REGIST_OK)){
            QMessageBox::information(this,"注册",REGIST_OK);
        }
        else if(0 == strcmp(pdu->caData,REGIST_FAILED)){
              QMessageBox::warning(this,"注册",REGIST_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:           //登录消息响应
    {
        if(0 == strcmp(pdu->caData,LOGIN_OK)){
            QMessageBox::information(this,"登录",LOGIN_OK);
             OpeWidget::getInstance().show();                   //登录成功显示具体操作界面，隐藏登录界面
            hide();
        }
        else if(0 == strcmp(pdu->caData,LOGIN_FAILED)){
              QMessageBox::warning(this,"登录",LOGIN_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:          //在线显示响应
    {
        OpeWidget::getInstance().getFriend()->showAllOnlineUsr(pdu);   //显示在线窗口
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:          //查找用户响应
    {
        if(0 == strcmp(SEARCH_USR_NO,pdu->caData)){                       //用户不存在数据库
            QMessageBox::information(this,"搜索",QString("%1: not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        else if(0 == strcmp(SEARCH_USR_ONLINE,pdu->caData)){            //用户存在且在线
            QMessageBox::information(this,"搜索",QString("%1: online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        else if(0 == strcmp(SEARCH_USR_OFFLINE,pdu->caData)){           //用户存在但是不在线
            QMessageBox::information(this,"搜索",QString("%1: offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        break;
    }
    default:
        break;
    }
    free(pdu);        //释放申请空间
    pdu = NULL;
}

#if 0                       //测试按钮发送代码，已弃用

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

void TcpClient::on_login_pb_clicked()       //点击登录按钮
{
    QString strName = ui->name_le->text();           //获取输入框内容姓名和密码
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty()){                //判断是否为空
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;          //设置消息类型为登录请求
        strncpy(pdu->caData,strName.toStdString().c_str(),32);            //将姓名和密码转换为32位字符数组存入pdu->caData
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);                             //发送数据至服务器
        free(pdu);
        pdu = NULL;

    }
    else{
        QMessageBox::critical(this,"登录","登录失败：用户名或者密码为空！");           //登录失败提醒
    }
}


void TcpClient::on_regist_pb_clicked()                         //注册按钮按下
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty()){                       //用户名密码均不为空
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;

    }
    else{
        QMessageBox::critical(this,"注册","注册失败：用户名或者密码为空！");     //注册失败提醒
    }
}


void TcpClient::on_cancel_pb_clicked()
{

}

