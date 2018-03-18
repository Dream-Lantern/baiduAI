#include <iostream>
#include <string>
#include <string.h>
#include <json/json.h>
#include <curl/curl.h>
#include "faceDetect.h"
#include "myJson.h"
#include "mySQL.h"
#include "fdfsUploadFile.h"

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
    std::string url = faceDetect::request_url + "?access_token=" + access_token;
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

// 将图片上传到 fdfs
int faceDetect::uploadFdfs(char* localFile)
{
    m_imgUrl[64] = {0};
    uploadFile* upFile = uploadFile::getInstance();
    // 此时 argv[1] 为 php保存到本地的 绝对路径
    string fileNameTmp = localFile;

    const char* fileName = fileNameTmp.c_str();
    int ret = upFile->myUploadFile(upFile->m_confFile, fileName, m_imgUrl, sizeof(m_imgUrl));
    if (ret != 0)
    {
        cout << "fdfs err" << endl;
        return -1;
    }
    return 0;
}

// 将图片信息保存到 mysql数据库中
int faceDetect::saveDB(const char* host, const char* user, const char* pswd, const char* dbName)
{
    // 获取 单例
    mySQL* mysql = mySQL::getInstance();
    // 建立连接
    mysql->m_sqlHandler = mysql->conn(host, user, pswd, dbName);
    if (mysql->m_sqlHandler == NULL)
    {
        cout << __FUNCTION__ << "real conn err" << endl;
    }
    // sql赋值
    Json::Value res = resJson();
    int age = res["age"].asInt();
    double beauty = res["beauty"].asDouble();
    string gender = res["gender"].asString();
    if (gender == "")
    {
        // json解析失败， 不插入数据库
        return -1;
    }
    const char* sqlGender = gender.c_str();
    int glasses = res["glasses"].asInt();
    string race = res["race"].asString();
    const char* sqlRace = race.c_str();

    // 将成员 m_imgUrl 的有效长度拷贝到字符串中
    char* imgUrl = (char*)calloc(sizeof(char), strlen(m_imgUrl));   
    // 51是 fdfsFileID 的长度
    memcpy(imgUrl, m_imgUrl, 51);

    char intertSql[256] = {0};

    sprintf(intertSql, "insert into face(age, beauty, gender, glass, race, url_img) values(%d, %lf, '%s', %d, '%s', '%s')", age, beauty, sqlGender, glasses, sqlRace, imgUrl);
    int ret = mysql->myQuery(intertSql);
    cout << res << endl;

    if (imgUrl != NULL)
    {
        free(imgUrl);
    }
    return ret;   
}

