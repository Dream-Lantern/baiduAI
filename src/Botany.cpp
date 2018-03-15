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

    // botany
    absBusiness* botany = new botanyDetect;
    int ret = botany->discern(accessToken, fileByte);
    if (ret != 0)
    {
        cout << "get res err!" << endl;
        return -1;
    }

    Json::Value res = botany->resJson();

    cout << res << endl;
    
    delete(botany);
	return 0;
}
