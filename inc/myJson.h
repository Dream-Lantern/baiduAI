#pragma once
#include <json/json.h>

class myJson
{
    public:
        // 解析 int 型 value
        int readJson_Int(const std::string & strValue, std::string key, int &value);
        // 解析 string 型 value
        int readJson_String(const std::string & strValue, std::string key, std::string &value);
        // 解析 数组中的 int
        int readJson_ArrayObj_Int(const std::string & strValue, std::string obj, std::string key, int &value);
        // 解析 数组中的 double
        int readJson_ArrayObj_Double(const std::string & strValue, std::string obj, std::string key, double &value);
        // 解析 数组中的 string
        int readJson_ArrayObj_String(const std::string & strValue, std::string obj, std::string key, std::string &value);
        // 解析 数组中的 bool
        int readJson_ArrayObj_Bool(const std::string & strValue, std::string obj, std::string key, bool &value);
        // 解析 数组中的 obj
        int readJson_ArrayObj_Obj(const std::string & strValue, std::string obj, std::string key, std::string keyObj, Json::Value &value);

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
