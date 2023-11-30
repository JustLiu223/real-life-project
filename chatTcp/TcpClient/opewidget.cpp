#include "opewidget.h"

OpeWidget::OpeWidget(QWidget *parent)        //登陆后的操作界面
    : QWidget{parent}
{
    m_pListW = new QListWidget(this);
    m_pListW->addItem("好友");
    m_pListW->addItem("图书");
    m_pFriend = new Friend;
    m_pBook = new Book;
    m_pSW  = new QStackedWidget;
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pBook);
    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_pSW);
    setLayout(pMain);
    connect(m_pListW,SIGNAL(currentRowChanged(int)),m_pSW,SLOT(setCurrentIndex(int)));       //选中行发生变化，切换到与选中行对应的新页面
}

OpeWidget &OpeWidget::getInstance()       //单例模式
{
    static OpeWidget instance;
    return instance;
}

Friend *OpeWidget::getFriend()                  //获取好友变量
{
    return m_pFriend;
}
