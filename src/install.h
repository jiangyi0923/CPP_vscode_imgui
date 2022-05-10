#pragma once

#include "settings.h"
extern settings 设置;

class install
{
private:
    /* data */
    void download(const char *Url, const char *save_as); /*将Url指向的地址的文件下载到save_as指向的本地文件*/
public:
    install(/* args */);
    ~install();
    void fordolo(){};
    bool 是卸载 = false;
    bool 是疑难安装 = false;
    void 卸载逻辑(){};
    float 项目进度(){return 0.0f;};
};
