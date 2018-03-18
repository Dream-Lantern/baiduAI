#pragma once

using namespace std;

class Judge
{
    public:
    // 获取accessToken, 读取文件, 转换base64
    string init(char* fileName);
    // 读取配置文件信息
    int readConf(string path);

    // 存储二进制文件
    string m_uploadByte;
    char *m_fileByte;
    char *m_fileBase64;
    // 存储 accessToken
    string m_accessToken;
    // 存储 数据库的信息
    string m_host;
    string m_user;
    string m_passwd;
    string m_dbName;

    // 析构, 释放 读取文件的字节流
    ~Judge();

};
