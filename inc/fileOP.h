#pragma once

class fileOP
{
    public:
        // 获取文件的长度
        int getFileLen(char *path);
        // 读取文件内容, 传出字节流
        int readFile(char *path, char* out, int len);
        // 获取单例的方法
        static fileOP* getInstance();
    private:
        // 将构造函数 和 析构函数私有化, 饿汉单例模式
        fileOP()
        {
        }
        fileOP(const fileOP &file)
        {
        }
        static fileOP *m_file;
};

