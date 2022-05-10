#pragma once

#include "settings.h"
extern settings 设置;

class install
{
private:
    /* data */
    void download(settings::插件 * 插件插件); /*将Url指向的地址的文件下载到save_as指向的本地文件*/
    void download_S(settings::插件 * 插件插件);
    bool chickdxneid();
    void 冲突检测();
    void 安装逻辑_线程();
    void 卸载逻辑_线程();
    void 安装逻辑_线程_疑难();

public:
    install(/* args */);
    ~install();
    void fordolo();
    bool 是卸载 = false;
    bool 是疑难安装 = false;
    void 卸载逻辑();
    bool md5chick2(string path, string md5v);
    bool unzipfs_ss(string FileDir, string outFileDir);
    void 安装逻辑();
};
