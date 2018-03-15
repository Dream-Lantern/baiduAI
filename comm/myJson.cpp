#include <iostream>
#include <string>
#include <json/json.h>
#include "myJson.h"
using namespace std;

// 单例模式
myJson* myJson::mj = new myJson;
myJson* myJson::getInstance()
{
    return mj;
}

/**
 * 解析value为int类型的json
 * @param1 json字符串
 * @param2 key
 * @param3 value 传出
 */
int myJson::readJson_Int(const std::string & strValue, std::string key, int &value)
{
    Json::Reader reader;  
    Json::Value  root;
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        value = root[key].asInt();
        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * 解析value为string类型的json
 * @param1 json字符串
 * @param2 key
 * @param3 value 传出
 */
int myJson::readJson_String(const std::string & strValue, std::string key, std::string &value)
{
    Json::Reader reader;  
    Json::Value  root;
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        value  = root[key].asString();
        ret = 0;
    }

    return ret;
}

/**
 * 解析value为string类型的json
 * @param1 json字符串
 * @param2 json对象
 * @param3 key
 * @param4 value 传出
 * @param5 len 默认参数，默认解析1个数组长度
 */
int myJson::readJson_ArrayObj_Int(const std::string & strValue, std::string obj, std::string key, int &value, int len)
{
    Json::Reader reader;  
    Json::Value  root; 
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        Json::Value arrayObj = root[obj];
        //for(unsigned int i = 0; i < arrayObj.size(); i++)
        for(int i = 0; i < len; i++)
        { 
            if (arrayObj[i].isMember(key))
            {
                value = arrayObj[i][key].asInt();
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
    }
    return ret;
}

/**
 * 解析value为double类型的json
 * @param1 json字符串
 * @param2 json对象
 * @param3 key
 * @param4 value 传出
 * @param5 len 默认参数，默认解析1个数组长度
 */
int myJson::readJson_ArrayObj_Double(const std::string & strValue, std::string obj, std::string key, double &value, int len)
{
    Json::Reader reader;  
    Json::Value  root; 
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        Json::Value arrayObj = root[obj];
        //for(unsigned int i = 0; i < arrayObj.size(); i++)
        for(int i = 0; i < len; i++)
        { 
            if (arrayObj[i].isMember(key))
            {
                value = arrayObj[i][key].asDouble();
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
    }
    return ret;
}

/**
 * 解析value为string类型的json
 * @param1 json字符串
 * @param2 json对象
 * @param3 key
 * @param4 value 传出
 * @param5 len 默认参数，默认解析1个数组长度
 */
int myJson::readJson_ArrayObj_String(const std::string & strValue, std::string obj, std::string key, std::string &value, int len)
{
    Json::Reader reader;  
    Json::Value  root; 
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        Json::Value arrayObj = root[obj];
        //for(unsigned int i = 0; i < arrayObj.size(); i++)
        for(int i = 0; i < len; i++)
        { 
            if (arrayObj[i].isMember(key))
            {
                value = arrayObj[i][key].asString();
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
    }
    return ret;
}

/**
 * 解析value为bool类型的json
 * @param1 json字符串
 * @param2 json对象
 * @param3 key
 * @param4 value 传出
 * @param5 len 默认参数，默认解析1个数组长度
 */
int myJson::readJson_ArrayObj_Bool(const std::string & strValue, std::string obj, std::string key, bool &value, int len)
{
    Json::Reader reader;  
    Json::Value  root; 
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        Json::Value arrayObj = root[obj];
        //for(unsigned int i = 0; i < arrayObj.size(); i++)
        for(int i = 0; i < len; i++)
        { 
            if (arrayObj[i].isMember(key))
            {
                value = arrayObj[i][key].asBool();
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
    }
    return ret;
}

/**
 * json数组中嵌套一个json对象
 * @param5 len 默认参数，默认解析1个数组长度
 */
int myJson::readJson_ArrayObj_Obj(const std::string & strValue, std::string obj, std::string key, std::string keyObj, Json::Value &value, int len)
{
    Json::Reader reader;  
    Json::Value  root; 
    int ret = -1;

    if (reader.parse(strValue, root))
    {
        Json::Value arrayObj = root[obj];
        //for(unsigned int i = 0; i < arrayObj.size(); i++)
        for(int i = 0; i < len; i++)
        { 
            if (arrayObj[i].isMember(key))
            {
                // 解析 数组中的另一个 json对象
                Json::Value valueObj = arrayObj[i][key];
                //Json::Value subCoat = valueObj[keyObj];
                value = valueObj[keyObj];
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
    }
    return ret;
}

