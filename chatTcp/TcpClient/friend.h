#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "online.h"
#include <QTableWidget>

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    void showAllOnlineUsr(PDU *pdu);
    void updateFriendList(PDU *pdu);
    QString m_strSearchName;                        //查找用户姓名
public slots:
    void showOnline();
    void searchUsr();
    void flushFriend();
    void deleteFriend();
    void privateChat();
signals:
private:
    QTextEdit *m_pShowMsgTE;
    // QListWidget *m_pFriendListWidget;
    QTableWidget * m_pFriendListWiget;
    QLineEdit *m_pInputMsgLE;
    QPushButton *m_pDelFriendPB;
    QPushButton *m_pFlashFriendPB;
    QPushButton *m_pShowOnlineUsrPB;
    QPushButton *m_pSearchUsrPB;
    QPushButton *m_pMsgSendPB;
    QPushButton *m_pPrivateChatPB;
    Online *m_pOnline;

};

#endif // FRIEND_H
