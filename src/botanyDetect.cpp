#include <iostream>
#include <string>
#include <string.h>
#include <curl/curl.h>
#include <json/json.h>
#include "botanyDetect.h"
#include "myJson.h"
#include "mySQL.h"
#include "fdfsUploadFile.h"

// 植物识别 请求的 url
const std::string botanyDetect::request_url = "https://aip.baidubce.com/rest/2.0/image-classify/v1/plant";
// 存储 返回 json的 静态变量
std::string botanyDetect::detect_result;
/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
 * @param 参数定义见libcurl文档
 * @return 返回值定义见libcurl文档
 */
size_t botanyDetect::callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    // 获取到的body存放在ptr中，先将其转换为string格式
    botanyDetect::detect_result = std::string((char *) ptr, size * nmemb);
    return size * nmemb;
}
/**
 * 植物识别
 * @return 调用成功返回0，发生错误返回其他错误码
 */
int botanyDetect::discern(const std::string &access_token, const std::string &base64Buf) 
{
    std::string url = botanyDetect::request_url + "?access_token=" + access_token;
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
        m_jsonRes = botanyDetect::detect_result;
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
Json::Value botanyDetect::resJson()
{
    myJson* myjson = myJson::getInstance();
    // 按协议 解析出需要的 value 并返回
    int ret = -1;
    string resName;
    ret = myjson->readJson_ArrayObj_String(m_jsonRes, "result", "name", resName);

    //string reScore;
    //ret = myjson->readJson_ArrayObj_String(m_jsonRes, "result", "score", reScore);
    double reScore;
    ret = myjson->readJson_ArrayObj_Double(m_jsonRes, "result", "score", reScore);

    Json::Value res;
    if (ret == 0)
    {
        res["name"] = resName;
        res["score"] = reScore;
        // 判断解析的 json 合法性, 如果为空, 说明 鉴别 失败
        if (resName.size() > 0)
        {
            cout << "识别成功 ^_^ " << endl;
            return res;
        }
    }
    cout << "识别失败, 请重新上传 >_< " << endl;
    return res;
}

// 将图片上传到 fdfs
int botanyDetect::uploadFdfs(char* localFile)
{
    m_imgUrl[64] = {0};
    uploadFile* upFile = uploadFile::getInstance();
    // 此时 argv[1] 为 php保存到本地的 绝对路径
    string fileNameTmp = localFile;

    const char* fileName = fileNameTmp.c_str();
    // m_imgUrl 为传出参数, 被赋值为 图片的url
    int ret = upFile->myUploadFile(upFile->m_confFile, fileName, m_imgUrl, sizeof(m_imgUrl));
    if (ret != 0)
    {
        cout << "fdfs err" << endl;
        return -1;
    }
    return 0;
}

// 将图片信息保存到 mysql数据库中
int botanyDetect::saveDB(const char* host, const char* user, const char* pswd, const char* dbName)
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

    string resName = res["name"].asString();
    if (resName == "")
    {
        // json 解析失败， 不插入数据库
        return -1;
    }
    const char* sqlName = resName.c_str();
    double reScore = res["score"].asDouble();

    // 将成员 m_imgUrl 的有效长度拷贝到字符串中
    char* imgUrl = (char*)calloc(sizeof(char), strlen(m_imgUrl));   
    // 51是 fdfsFileID的长度
    memcpy(imgUrl, m_imgUrl, 51);

    // 存储sql语句
    char intertSql[256] = {0};

    sprintf(intertSql, "insert into botany(name, score, url_img) values('%s', %lf, '%s')", sqlName, reScore, imgUrl);
    int ret = mysql->myQuery(intertSql);
    cout << res << endl;

    if (imgUrl != NULL)
    {
        free(imgUrl);
    }
    return ret;   
}


