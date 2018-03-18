#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include "init.h"
#include "base64.h"
#include "getAccessToken.h"
#include "myJson.h"
#include "fileOP.h"

using namespace std;

// 读取配置文件信息
int Judge::readConf(string path)
{
    Json::Reader reader;
    Json::Value root;

    ifstream is;
    is.open(path, ios::binary);
    if (reader.parse(is, root))
    {
        // 成功解析
        // 赋值给 成员
        m_host = root["localhost"].asString();
        m_user = root["user"].asString();
        m_passwd = root["passwd"].asString();
        m_dbName = root["dbname"].asString();

        is.close();
        return 0;
    }
    else
    {
        // 解析失败
        is.close();
        return -1;
    }
}

// 完成 读取图片文件，获取 accesToken 工作
string Judge::init(char* fileName)
{
    int ret = -1;

    // 读取数据库配置文件信息
    ret = readConf("/home/gujiguji/baiduAI/src/baiduAI/conf/mysql.conf");
    if (ret != 0)
    {
        cout << "read sql.conf err" << endl;
        return NULL;
    }

    // 获取access
    getAccessToken *access = getAccessToken::getInstance();
    // 给成员 m_accessToken 赋值
    ret = access->get_access_token(m_accessToken, getAccessToken::API_Key, getAccessToken::Secure_Key);
    if (ret != 0)
    {
        cout << "getAccessToken err!" << endl;
        return NULL;
    }

    // 获取文件字节流
    fileOP *fp = fileOP::getInstance();
    int filelen = fp->getFileLen(fileName);
    m_fileByte = (char*)malloc(sizeof(char)*filelen);
    memset(m_fileByte, 0x00, filelen);

    ret = fp->readFile(fileName, m_fileByte, filelen);

    // base64编码
    m_fileBase64 = (char*)malloc(sizeof(char)*filelen*3/2);
    memset(m_fileBase64, 0x00, filelen*3/2);

    base64 *base = base64::getInstance();
    base->Base64Code(m_fileByte, m_fileBase64, filelen);

    // 返回string类型的引用, string容器内存放的时编码之后的 base64
    m_uploadByte = m_fileBase64;
    return m_uploadByte;
}
// 析构掉 在堆区 开辟的空间
Judge::~Judge()
{
    if (m_fileByte != NULL)
    {
        free(m_fileByte);
    }

    if (m_fileBase64 != NULL)
    {
        free(m_fileBase64);
    }
}

