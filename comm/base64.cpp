#include <iostream>
#include <string>
#include "base64.h"

using namespace std;

// base64编码字符串
const char* base64::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// 单例模式
base64* base64::m_base = new base64;
base64* base64::getInstance()
{
    return m_base;
}

/**
 * base64编码
 * @param1 传入字节流
 * @param2 传出字节流
 * @param3 传入字节流的长度
 */
void base64::Base64Code(char* chSrc, char* chDes, int len)
{
    unsigned char char_array_3[3], char_array_4[4];
    int i = 0, j = 0;

    while(len--)
    {
        char_array_3[i++] = *(chSrc++);
        if(3 == i)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for(i = 0; i < 4; i++)
                *(chDes+i) = base64::base64_chars[char_array_4[i]];

            i = 0;
            chDes += 4;
        }
    }
    if(i)
    {
        for(j = i; j < 3; j++)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for(j = 0; j < (i+1); j++)
        {
            *(chDes++) = base64::base64_chars[char_array_4[j]];
        }

        while((3 > i++))
        {
            *(chDes++) = '=';
        }
    }

    *chDes = '\0';

    return;
}   
