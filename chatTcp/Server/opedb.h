#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr); //?
    static OpeDB& getInstance();
    void init();
    ~OpeDB();
    bool handleRegist(const char *name, const char *pwd);
signals:
private:
    QSqlDatabase m_db; //连接数据库
};

#endif // OPEDB_H
