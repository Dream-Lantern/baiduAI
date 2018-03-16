#pragma once
#include <mysql/mysql.h>

// 单例
class mySQL
{
    public:
        // 获取单例
        static mySQL* getInstance();
        // 连接数据库
        MYSQL* conn(const char* host, const char* user, const char* pswd, const char* dbName);       
        // 执行sql语句
        int myQuery(char* sql);
        // 操作 mysql 的句柄
        MYSQL* m_sqlHandler;
    private:
        mySQL();
        ~mySQL();
        static mySQL* m_mysql;
};
