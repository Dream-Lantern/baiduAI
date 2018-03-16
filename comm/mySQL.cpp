#include <iostream>
#include <strings.h>
#include <string.h>
#include "mySQL.h"

using namespace std;

// 获取单例
mySQL* mySQL::m_mysql = new mySQL;
mySQL* mySQL::getInstance()
{
    return m_mysql;
}

// 构造 
mySQL::mySQL()
{
}

// 析构 
mySQL::~mySQL()
{
}

// 连接数据库
MYSQL* mySQL::conn(const char* host, const char* user, const char* pswd, const char* dbName)
{
    // 初始化 连接句柄
    m_sqlHandler = mysql_init(NULL);
    // 连接数据库
    if(mysql_real_connect(m_sqlHandler, host, user, pswd, dbName, 0, NULL, 0))
    {
        return m_sqlHandler;
    }
    else
    {
        return NULL;
    }
}

// 执行sql语句
int mySQL::myQuery(char* sql)
{

    int ret = mysql_query(m_sqlHandler, sql);
    mysql_close(m_sqlHandler);
    return ret;
}
