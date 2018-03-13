#pragma once

class getAccessToken
{
    public:
        // 获取 accessToken值
        int get_access_token(std::string &access_token, const std::string &AK, const std::string &SK);
        // 回调函数, 为获取的accessToken 赋值
        static size_t getToken(void *ptr, size_t size, size_t nmemb, void *stream);
        // 申请 accessToken 的 url
        static const std::string access_token_url;
        // API Key
        static const std::string API_Key;
        // Secure Key
        static const std::string Secure_Key;

        // 获取单例的方法
        static getAccessToken* getInstance();
    private:
        // 私有化 构造函数
        getAccessToken()
        {
        }
        getAccessToken(const getAccessToken& access)
        {
        }
        // 单例
        static getAccessToken* m_access;
};


