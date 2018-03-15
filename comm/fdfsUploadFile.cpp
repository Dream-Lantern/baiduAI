#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "fdfsUploadFile.h"

// new一个 唯一的 fdfsUP 类
uploadFile* uploadFile::m_up = new uploadFile;

// 获取单例
uploadFile* uploadFile::getInstance()
{
    return m_up;
}
// 为成员赋值
const char* uploadFile::m_confFile = "/home/gujiguji/baiduAI/src/baiduAI/fdfs/client/client.conf";

/** 在进程中启动fdfs_upload_file
 *  @param1 fdfs_client.conf
 *  @param2 上传的本地文件
 *  @param3 读出fdfs上传的文件ID
 */
int uploadFile::myUploadFile(const char* confFile, const char* localFile, char* fileID, int len)
{
    // 创建匿名管道
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }
    
    // 创建子进程
    pid_t pid = fork();
    if(pid == 0)
    {
        // 子进程
        // 写管道, 关闭读端
        close(fd[0]);
        // 重定向: 标准输出 -> 管道的写端
        dup2(fd[1], STDOUT_FILENO);
        // 执行execlp操作
        execlp("fdfs_upload_file", "fdfs_upload_file", confFile, localFile, NULL);

        // exec替换了 text段 data段, 没有返回, 执行到下面的代码说明报错了
        perror("execlp error");
        exit(0);
        // 关闭写管道
        close(fd[1]);
    }
    else if(pid > 0)
    {
        // 父进程
        // 读管道, 关闭写端
        close(fd[1]);
        read(fd[0], fileID, len);
        wait(NULL);
        // 关闭读管道
        close(fd[0]);
    }
    return 0;
}
