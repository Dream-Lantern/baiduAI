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

};


