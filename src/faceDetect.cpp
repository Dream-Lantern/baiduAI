#include <iostream>
#include <string>
#include <json/json.h>
#include <curl/curl.h>
#include "faceDetect.h"
#include "myJson.h"

using namespace std;

// 请求的url
const std::string faceDetect::request_url = "https://aip.baidubce.com/rest/2.0/face/v2/detect";
// 声明 存储json的变量
std::string faceDetect::detect_result;

/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
 * @param 参数定义见libcurl文档
 * @return 返回值定义见libcurl文档
 */
size_t faceDetect::callback(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    // 获取到的body存放在ptr中，先将其转换为string格式
    faceDetect::detect_result = std::string((char *) ptr, size * nmemb);
    return size * nmemb;
}

/**
 * 人脸探测
 * @return 调用成功返回0，发生错误返回其他错误码
 */
//int faceDetect::detect(std::string &json_result, const std::string &access_token, const std::string &base64Buf) {
int faceDetect::discern(const std::string &access_token, const std::string &base64Buf) {
    std::string url = request_url + "?access_token=" + access_token;
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
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "max_face_num", CURLFORM_COPYCONTENTS, "5", CURLFORM_END);
        //curl_formadd(&post, &last, CURLFORM_COPYNAME, "face_fields", CURLFORM_COPYCONTENTS, "age,beauty,expression,faceshape,gender,landmark,glasses,race,qualities", CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "face_fields", CURLFORM_COPYCONTENTS, "age,beauty,gender,glasses,race,qualities", CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "image", CURLFORM_COPYCONTENTS, base64Buf.data(), CURLFORM_END);

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
        //json_result = faceDetect::detect_result;
        // 给成员 m_jsonRes 赋值
        m_jsonRes = faceDetect::detect_result;
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

// json 解析的结果 , 参数为 需要解析的 key值
Json::Value faceDetect::resJson()
{
    myJson* myjson = myJson::getInstance();
    // 按协议 解析出需要的 value 并返回
    int ret = -1;

    int resAge = 0;
    ret = myjson->readJson_ArrayObj_Int(m_jsonRes, "result", "age", resAge);

    double resBeauty = 0;
    ret = myjson->readJson_ArrayObj_Double(m_jsonRes, "result", "beauty", resBeauty);

    string resGender;
    ret = myjson->readJson_ArrayObj_String(m_jsonRes, "result", "gender", resGender);

    string resRace;
    ret = myjson->readJson_ArrayObj_String(m_jsonRes, "result", "race", resRace);

    int resGlasses = 0;
    ret = myjson->readJson_ArrayObj_Int(m_jsonRes, "result", "glasses", resGlasses);

    Json::Value resType;
    ret = myjson->readJson_ArrayObj_Obj(m_jsonRes, "result", "qualities", "type", resType);

    // parse succ!
    Json::Value res;
    if (ret == 0)
    {
        res["age"] = resAge;
        res["beauty"] = resBeauty;
        res["gender"] = resGender;
        res["race"] = resRace;
        res["glasses"] = resGlasses;
        res["type"] = resType;
        // 判断解析的 json 合法性, 如果为空, 说明 鉴别 失败
        if (resGender.size() > 0)
        {
            cout << "识别成功 ^_^ " << endl;
            return res;
        }
    }
    cout << "识别失败, 请重新上传 >_< " << endl;
    return res;
}
