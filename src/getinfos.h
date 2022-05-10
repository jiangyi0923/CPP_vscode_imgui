#pragma once
#include "GITEEJSON.h"
#include "GITEEboyd.h"
#include "settings.h"

extern settings 设置;



class getinfos
{
private:
    void getfileinfo(string filebody);
    string downloadinfo(const char *Url);
    tm gettime();

    void toget();
    void toget_lc();

public:
    getinfos(/* args */);
    ~getinfos();
    bool thisonce = true;

    void getto(int f);
};

