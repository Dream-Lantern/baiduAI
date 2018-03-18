#! /bin/bash

# 如果 fdfs 已经启动 则重启
# 如果 fdfs 未启动 则启动

# tracker
ps aux | grep fdfs_trackerd | grep -v grep > /dev/null
# 说明启动, 重启
if [ $? -eq 0 ]
then
    sudo fdfs_trackerd /home/gujiguji/baiduAI/src/baiduAI/fdfs/tracker/tracker.conf restart
    if [ $? -eq 0 ]
    then
        echo "fdfs_trackerd restart succ ^_^ "
    else
        echo "fdfs_trackerd restart fail >_< "
    fi
# 说明未启动, 启动
else
    sudo fdfs_trackerd /home/gujiguji/baiduAI/src/baiduAI/fdfs/tracker/tracker.conf start
    if [ $? -eq 0 ]
    then
        echo "fdfs_trackerd start succ ^_^ "
    else
        echo "fdfs_trackerd start fail >_< "
    fi
fi

# storage
ps aux | grep fdfs_storaged | grep -v grep > /dev/null
# 说明启动, 重启
if [ $? -eq 0 ]
then
    sudo fdfs_storaged /home/gujiguji/baiduAI/src/baiduAI/fdfs/storage/storage.conf restart
    if [ $? -eq 0 ]
    then
        echo "fdfs_storaged restart succ ^_^ "
    else
        echo "fdfs_storaged restart fail >_< "
    fi
# 说明未启动, 启动
else
    sudo fdfs_storaged /home/gujiguji/baiduAI/src/baiduAI/fdfs/storage/storage.conf start
    if [ $? -eq 0 ]
    then
        echo "fdfs_storaged start succ ^_^ "
    else
        echo "fdfs_storaged start fail >_< "
    fi
fi
