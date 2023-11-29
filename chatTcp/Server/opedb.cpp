#include "opedb.h"
#include <QMessageBox>
#include <QDebug>
OpeDB::OpeDB(QObject *parent)
    : QObject{parent}
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

OpeDB &OpeDB::getInstance()  //单例模式
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("D:\\github\\real-life-project\\chatTcp\\Server\\cloud.db");
    if(m_db.open())
    {
        QSqlQuery query;
        query.exec("select * from userInfo");
        while(query.next()){
            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            qDebug()<<data;
        }
    }
    else
    {
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }
}

OpeDB::~OpeDB()
{
    m_db.close();
}

bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    if(NULL == name || NULL ==pwd)
    {
        return false;
    }
    QString data = QString("insert into userInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query;
    return query.exec(data);
}
