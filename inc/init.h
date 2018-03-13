#pragma once

using namespace std;

class Judge
{
    public:
    // 获取accessToken, 读取文件, 转换base64
    string init(char* fileName);

    // 存储二进制文件
    string m_uploadByte;
    char *m_fileByte;
    char *m_fileBase64;

    // 析构, 释放掉 读取文件的字节流
    ~Judge();

};
