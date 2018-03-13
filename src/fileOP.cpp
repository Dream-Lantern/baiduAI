#include <stdio.h>
#include <iostream>
#include "fileOP.h"

using namespace std;

// 单例模式
fileOP* fileOP::m_file = new fileOP;
fileOP* fileOP::getInstance()
{
    return m_file;
}

/**
 * 获取文件长度， 用来确认在堆区开辟空间
 * @param 文件路径
 * @return 文件长度
 */
int fileOP::getFileLen(char *path)
{
    FILE *fp;
    int length;

    fp = fopen(path,"r");
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
    return length;
}

/**
 * 读取文件内容
 * @param1 文件路径
 * @param2 传出字节流
 * @param3 文件长度
 * @return 读取成功的长度
 */
int fileOP::readFile(char *path, char* out, int len)
{
    FILE *fp;
    int ret;

    fp = fopen(path, "r");
    if(fp==NULL)
    {
        cout << "getFileLen open file err" << endl;
    }

    // 逐个字节读取
    ret = fread(out, 1, len, fp);
    if(ret == 0)
    {
        cout<<"Read_Image fread err" <<endl;
    }
    fclose(fp);

    return ret;
}
