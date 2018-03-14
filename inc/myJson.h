#pragma once
#include <json/json.h>

class myJson
{
    public:
        // 解析 int 型 value
        int readJson_Int(const std::string & strValue, std::string key, int &value);
        // 解析 string 型 value
        int readJson_String(const std::string & strValue, std::string key, std::string &value);
        // 解析 数组中的 int  最后一个参数为默认解析数组的长度, 
        int readJson_ArrayObj_Int(const std::string & strValue, std::string obj, std::string key, int &value, int len = 1);
        // 解析 数组中的 double 最后一个参数为默认解析数组的长度 
        int readJson_ArrayObj_Double(const std::string & strValue, std::string obj, std::string key, double &value, int len = 1);
        // 解析 数组中的 string 最后一个参数为默认解析数组的长度 
        int readJson_ArrayObj_String(const std::string & strValue, std::string obj, std::string key, std::string &value, int len = 1);
        // 解析 数组中的 bool 最后一个参数为默认解析数组的长度 
        int readJson_ArrayObj_Bool(const std::string & strValue, std::string obj, std::string key, bool &value, int len = 1);
        // 解析 数组中的 obj 最后一个参数为默认解析数组的长度 
        int readJson_ArrayObj_Obj(const std::string & strValue, std::string obj, std::string key, std::string keyObj, Json::Value &value, int len = 1);

        // 获取 单例方法
        static myJson* getInstance();

    private:
        // 私有化 构造函数
        myJson()
        {
        }
        myJson(const myJson &json)
        {
        }
        static myJson* mj;
};

/*
*/
