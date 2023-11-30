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

void OpeDB::init()          //服务器初始化
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("D:\\github\\real-life-project\\chatTcp\\Server\\cloud.db");
    if(m_db.open())           //打开数据库并读取userInfo表中所有数据
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

bool OpeDB::handleRegist(const char *name, const char *pwd)         //数据库注册操作
{
    if(NULL == name || NULL ==pwd)    //数据检查
    {
        return false;
    }
    QString data = QString("insert into userInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);          //在数据库中插入数据
    QSqlQuery query;
    return query.exec(data);
}

bool OpeDB::handleLogin(const char *name, const char *pwd)              //数据库登录操作
{
    if(NULL == name || NULL ==pwd)
    {
        return false;
    }
    QString data = QString("select * from userInfo where name=\'%1\' and pwd = \'%2\' and online=0").arg(name).arg(pwd);
    QSqlQuery query;
    query.exec(data);
    if(query.next())           //检索符合条件的下一个
    {
        QString data = QString("update  userInfo set online=1 where name=\'%1\' and pwd = \'%2\' ").arg(name).arg(pwd);  //更新登录状态
        QSqlQuery query;
        query.exec(data);
        return true;
    }
    else{
        return false;
    }
}

void OpeDB::handleOffline(const char *name)
{
    if(NULL == name)
    {
        return;
    }
    QString data = QString("update  userInfo set online=0 where name=\'%1\' ").arg(name);
    QSqlQuery query;
    query.exec(data);
}

QStringList OpeDB::handleAllOnline()                 //数据库检索用户在线状态
{
    QString data = QString("select name from userInfo where online=1");          //查询online=1的用户，即为在线用户
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();
    while(query.next()){
        result.append(query.value(0).toString());         //查询结果保存在result中返回用户名
    }
    return result;
}

int OpeDB::handleSearchUsr(const char *name)                  //检索姓名符合的用户通过在线状态查找
{
    if(NULL == name){
        return -1;
    }
    QString data = QString("select online from userInfo where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);
    if(query.next()){                    //返回三种情况，在线，不在线，不存在
        int ret = query.value(0).toInt();
        if (ret ==1){
            return 1;
        }
        else if (ret == 0){
            return 0;
        }
    }
    else{
        return  -1;
    }
}
