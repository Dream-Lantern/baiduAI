#pragma once

class uploadFile
{
    // 单例 私有化 构造函数
    public:
        int myUploadFile(const char* confFile, const char* localFile, char* fileID, int len);

    static uploadFile* getInstance();
    const static char* m_confFile;

    private:
        uploadFile()
        {
        }
        uploadFile(const uploadFile &up)
        {
        
        }
        static uploadFile *m_up;
};
