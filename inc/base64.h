#pragma once

class base64
{
    public:
        // base64 encode
        void Base64Code(char* chSrc, char* chDes, int len);
        // base64 编码的字符串
        static const char* base64_chars;
        // 获取单例的方法
        static base64* getInstance();

    private:
        // 私有化 构造函数 单例模式
        base64()
        {
        }
        base64(const base64 &base)
        {
        }

        static base64* m_base;
};


