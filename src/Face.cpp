#include <iostream>
#include <string>
#include <strings.h>
#include <curl/curl.h>
#include <json/json.h>
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
     
    //face
    absBusiness* face = new faceDetect;
    int ret = face->discern(accessToken, fileByte);
    if (ret != 0)
    {
        cout << "get res err!" << endl;
        return -1;
    }

    Json::Value res = face->resJson();
    cout << res << endl;

    // 上传 图片到 fdfs,  fileId 为 fdfs里存储的文件名称
    char fileId[64] = {0};
    uploadFile* upFile = uploadFile::getInstance();
    // 此时 argv[1] 为 php保存到本地的 绝对路径
    string fileNameTmp = argv[1];

    const char* fileName = fileNameTmp.c_str();
    ret = upFile->myUploadFile(upFile->m_confFile, fileName, fileId, sizeof(fileId));
    if (ret != 0)
    {
        cout << "fdfs err" << endl;
        delete(face);
        return -1;
    }
    
    delete(face);
	return 0;
}
