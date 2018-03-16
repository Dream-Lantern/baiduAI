#pragma once
#include <iostream>
#include <string>

using namespace std;

class absBusiness
{
    public:
    virtual int discern(const std::string &access_token, const std::string &base64Buf) = 0;

    // 解析json结果
    virtual Json::Value resJson() = 0;

    // 将图片上传到fdfs
    virtual int uploadFdfs(char* localFile) = 0;   

    // 将信息存储到数据库中
    virtual int saveDB(const char* host, const char* user, const char* pswd, const char* dbName) = 0;

    // 虚析构
    virtual ~absBusiness()
    {
    }
};


