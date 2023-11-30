#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include "opewidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class TcpClient;
}
QT_END_NAMESPACE

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadconfig();
    static TcpClient &getInstance();
    QTcpSocket &getTcpSocket();
public slots:
    void showConnect();
    void recvMsg();



private slots:
    // void on_pushButton_clicked();

    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();

private:
    Ui::TcpClient *ui;
    QString m_strIP;             //ip地址
    quint16 m_usPort;           //端口
    QTcpSocket m_tcpSocket;     //stocket变量
};
#endif // TCPCLIENT_H
