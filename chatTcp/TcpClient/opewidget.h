#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "friend.h"
#include "book.h"
#include <QStackedWidget>
class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    static OpeWidget &getInstance();
    Friend *getFriend();


signals:
private:
    QListWidget  *m_pListW;
    Friend *m_pFriend;        //用户对象
    Book *m_pBook;
    QStackedWidget *m_pSW;

};

#endif // OPEWIDGET_H
