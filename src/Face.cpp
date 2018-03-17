#include <iostream>
#include <string>
#include <strings.h>
#include <curl/curl.h>
#include <json/json.h>
#include <stdio.h>
#include "base64.h"
#include "fileOP.h"
#include "getAccessToken.h"
#include "myJson.h"
#include "init.h"
#include "faceDetect.h"
#include "absBusiness.h"
#include "foodDetect.h"
#include "animalDetect.h"
#include "botanyDetect.h"
#include "fdfsUploadFile.h"
#include "mySQL.h"

using namespace std;

int main(int argc, char *argv[])
{
    // 读取文件信息
    Judge judge;
    string fileByte = judge.init(argv[1]);

    // 获取 access
    string accessToken;
    getAccessToken *access = getAccessToken::getInstance();
    access->get_access_token(accessToken, getAccessToken::API_Key, getAccessToken::Secure_Key);

    // new 一个业务对象 face
    absBusiness* face = new faceDetect;
    int ret = face->discern(accessToken, fileByte);
    if (ret != 0)
    {
        cout << "get res err!" << endl;
        return -1;
    }

    // 上传 图片到 fdfs,  fileId 为 fdfs里存储的文件名称
    ret = face->uploadFdfs(argv[1]);
    if (ret != 0)
    {
        delete(face);
        return -1;
    }

    // TODO 此处应当从配置文件中读取 IP等信息
   
    // 将图片url, json数据存储到 mysql
    const char* localhost = "localhost";
    const char* user = "root";
    const char* pswd = "qwe123";
    const char* dbName = "discern_img";
    ret = face->saveDB(localhost, user, pswd, dbName);
    if (ret != 0)
    {
        //cout << "insert err" << endl;
        // 插入数据库失败
        delete(face);
        return -1;
    }
    //cout << "insert succ" << endl;
    delete(face);

    return 0;
}
