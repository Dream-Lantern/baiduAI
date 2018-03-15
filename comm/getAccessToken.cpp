#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include "getAccessToken.h"

using namespace std;

// 单例模式
getAccessToken* getAccessToken:: m_access = new getAccessToken;
getAccessToken* getAccessToken:: getInstance()
{
    return m_access;
}

// 初始化Access的 url及key
const std::string getAccessToken::access_token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials";
const std::string getAccessToken::API_Key = "SIgr2k3Y1hTeMelNNEnDmhLW";
const std::string getAccessToken::Secure_Key = "NGQqc6rXXsP8yqZMtn62VxQWO7D4PGP9";

/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在result中
 * @param 参数定义见libcurl库文档
 * @return 返回值定义见libcurl库文档
 */
size_t getAccessToken::getToken(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    // 获取到的body存放在ptr中，先将其转换为string格式
    std::string s((char *) ptr, size * nmemb);
    // 开始获取json中的access token项目
    Json::Reader reader;
    Json::Value root;
    // 使用boost库解析json
    reader.parse(s,root);
    std::string* access_token_result = static_cast<std::string*>(stream);
    *access_token_result = root["access_token"].asString();
    return size * nmemb;
}

/**
 * 用以获取access_token的函数，使用时需要先在百度云控制台申请相应功能的应用，获得对应的API Key和Secret Key
 * @param access_token 获取得到的access token，调用函数时需传入该参数
 * @param AK 应用的API key
 * @param SK 应用的Secret key
 * @return 返回0代表获取access token成功，其他返回值代表获取失败
 */
int getAccessToken::get_access_token(std::string &access_token, const std::string &AK, const std::string &SK) 
{
    CURL *curl;
    CURLcode result_code;
    int error_code = 0;
    curl = curl_easy_init();
    if (curl)
    {
        std::string url = getAccessToken::access_token_url + "&client_id=" + AK + "&client_secret=" + SK;
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        std::string access_token_result;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &access_token_result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getAccessToken::getToken);
        result_code = curl_easy_perform(curl);
        if (result_code != CURLE_OK) 
	{
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(result_code));
            return 1;
        }
        access_token = access_token_result;
        curl_easy_cleanup(curl);
        error_code = 0;
    } 
    else
    {
        fprintf(stderr, "curl_easy_init() failed.");
        error_code = 1;
    }
    return error_code;
}
