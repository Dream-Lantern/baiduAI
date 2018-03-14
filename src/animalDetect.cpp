#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include "animalDetect.h"
#include "myJson.h"

// 动物 url
const std::string animalDetect::request_url = "https://aip.baidubce.com/rest/2.0/image-classify/v1/animal";
// 声明 存储json数据的变量
std::string animalDetect::detect_result;

/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
 * @param 参数定义见libcurl文档
 * @return 返回值定义见libcurl文档
 */
size_t animalDetect::callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    // 获取到的body存放在ptr中，先将其转换为string格式
    animalDetect::detect_result = std::string((char *) ptr, size * nmemb);
    return size * nmemb;
}
/**
 * 动物识别
 * @return 调用成功返回0，发生错误返回其他错误码
 */
int animalDetect::discern(const std::string &access_token, const std::string &base64Buf) 
{
    std::string url = animalDetect::request_url + "?access_token=" + access_token;
    CURL *curl = NULL;
    CURLcode result_code;
    int is_success;
    curl = curl_easy_init();
    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_httppost *post = NULL;
        curl_httppost *last = NULL;
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "image", CURLFORM_COPYCONTENTS, base64Buf.data(), CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "top_num", CURLFORM_COPYCONTENTS, "6", CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        result_code = curl_easy_perform(curl);
        if (result_code != CURLE_OK) 
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(result_code));
            is_success = 1;
            return is_success;
        }
        // 给成员 m_jsonRes赋值, 返回的结果存储在这个变量中
        m_jsonRes = animalDetect::detect_result;
        curl_easy_cleanup(curl);
        is_success = 0;
    }
    else
    {
        fprintf(stderr, "curl_easy_init() failed.");
        is_success = 1;
    }
    return is_success;
}

// json 解析的结果 
Json::Value animalDetect::resJson()
{
    myJson* myjson = myJson::getInstance();
    // 按协议 解析出需要的 value 并返回
    int ret = -1;
    string resName;
    ret = myjson->readJson_ArrayObj_String(m_jsonRes, "result", "name", resName);

    string reScore;
    ret = myjson->readJson_ArrayObj_String(m_jsonRes, "result", "score", reScore);

    Json::Value res;
    res["name"] = resName;
    res["score"] = reScore;
    // 判断解析的 json 合法性, 如果为空, 说明 鉴别 失败
    if (resName.size() > 0)
    {
        cout << "识别成功 ^_^ " << endl;
        return res;
    }
    cout << "识别失败, 请重新上传 >_< " << endl;
    return res;

}
