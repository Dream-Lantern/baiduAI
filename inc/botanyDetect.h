#pragma once
#include <iostream>
#include <json/json.h>
#include "absBusiness.h"
#include "mySQL.h"

using namespace std;

class botanyDetect : public absBusiness
{
    public:
    // 植物识别 api
    virtual int discern(const std::string &access_token, const std::string &base64Buf);

    // 植物识别 回调
    static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream);

    // 解析json结果
    virtual Json::Value resJson();

    // 将图片上传到fdfs
    virtual int uploadFdfs(char* localFile);   

    // 将信息存储到数据库
    virtual int saveDB(const char* host, const char* user, const char* pswd, const char* dbName);

    // 静态变量, 存储请求返回的 json字符串
    static std::string detect_result;
    // 静态变量，存储请求的 url
    static const std::string request_url;

    // 存储 解析json结果的 成员
    string m_jsonRes;
    // 存储图片 url
    char m_imgUrl[64];
};
