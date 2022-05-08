#pragma once
#include "GITEEJSON.h"
#include "GITEEboyd.h"

#include "global.h"
#include <stdio.h>

#include <io.h>

class getinfos
{
private:
    /* data */
    // string response;
    void getfileinfo(string filebody);
    string downloadinfo(const char *Url);
    tm gettime();

public:
    getinfos(/* args */);
    ~getinfos();
    void toget();
};

getinfos::getinfos(/* args */)
{
}

getinfos::~getinfos()
{
}

tm getinfos::gettime()
{
    tm buf;
    time_t t = time(NULL);
    localtime_s(&buf, &t);
    return buf;
}

string getinfos::downloadinfo(const char *Url) /*将Url指向的地址的文件下载到save_as指向的本地文件*/
{
    string response;
    try
    {
        设置.addlog("从服务器获取API数据...");
        BYTE Temp[1024];
        ULONG Number = 1;

        FILE *stream;
        HINTERNET hSession = InternetOpenA("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (hSession != NULL)
        {
            HINTERNET handle2 = InternetOpenUrlA(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
            if (handle2 != NULL)
            {
                设置.addlog("从服务器获取API数据成功");
                //设置.addlog("1");
                if (fopen_s(&stream, "jsoninfo.json", "w+b") != EINVAL)
                {
                    //设置.addlog("2");
                    // int err = 0;
                    while (Number > 0)
                    {
                        //设置.addlog("3");
                        InternetReadFile(handle2, Temp, 1024 - 1, &Number);
                        // printf_s(to_string(Number).c_str());
                        // err += Number + 1;
                        fwrite(Temp, sizeof(char), Number, stream);
                    }
                    // response << stream;
                    //设置.addlog("4");
                    fclose(stream);
                    //设置.addlog("5");
                    FILE *stream2;
                    设置.addlog("写入临时文件...");
                    fopen_s(&stream2, "jsoninfo.json", "r");
                    //设置.addlog("6");
                    struct stat sb
                    {
                    };
                    stat("jsoninfo.json", &sb);
                    //设置.addlog(std::to_string(sb.st_size) );
                    //设置.addlog(std::to_string(err) );
                    //设置.addlog("7");
                    response.resize(sb.st_size);
                    fread(const_cast<char *>(response.data()), sb.st_size, 1, stream2);
                    //.addlog("8");
                    fclose(stream2);
                }

                InternetCloseHandle(handle2);
                handle2 = NULL;
            }
            InternetCloseHandle(hSession);
            hSession = NULL;
            设置.addlog("从服务器获取API数据方法完成");
        } /* code */
    }
    catch (const std::exception &e)
    {
        // std::cerr << e.what() << '\n';
        设置.addlog(e.what());
        设置.apierro = 101;
        设置.addlog("从服务器获取API数据方法失败");
    }

    //设置.addlog("9");
    //设置.addlog(response);
    return response;
}

void getinfos::getfileinfo(string filebody)
{
    设置.addlog("开始解析获取的插件信息数据");
    // writelog(filebody);
    try
    {
        quicktype2::GiteEboyd data = nlohmann::json::parse(filebody);
        设置.fileday = data.get_fileday();
        设置.y = data.get_yer();
        设置.m = data.get_mos();
        设置.d = data.get_days();
        设置.dmd5s = data.get_dxmd5();
        //设置.addlog(to_string(gettime().tm_year));
        if (gettime().tm_year + 1900 > 设置.y)
        {
            printf("%d", gettime().tm_year + 1900);
            设置.限制安装 = false;
        }
        else
        {
            if ((gettime().tm_mon * 1 == 设置.m * 1 && gettime().tm_mday * 1 >= 设置.d * 1) || gettime().tm_mon * 1 > 设置.m * 1)
            {
                设置.限制安装 = false;
            }
            else
            {
                设置.限制安装 = true;
            }
        }

        for (size_t i = 0; i < data.get_files().size(); i++)
        {
            for (size_t t = 0; t < 设置.全部插件数据.size(); t++)
            {
                if (data.get_files()[i].get_id() == 设置.全部插件数据[t].id)
                {
                    设置.全部插件数据[t].Internal_name = data.get_files()[i].get_name();
                    设置.全部插件数据[t].file_md5 = data.get_files()[i].get_md5();
                    设置.全部插件数据[t].file_size = data.get_files()[i].get_size();
                }
            }
        }
        设置.addlog("解析插件信息数据方法完成!");
    }
    catch (...)
    {
        设置.apierro = 201;
        设置.addlog("解析插件信息数据方法失败!");
    }
}

void getinfos::toget()
{
    设置.apierro = 0;
    try
    {

        /* code */
        quicktype::Giteejson data = nlohmann::json::parse(downloadinfo("https://gitee.com/api/v5/repos/jiangyi0923/gw2chajianfile/releases/latest?access_token=ea9e8776dbc01bd019ca905d0418c6bf"));
        getfileinfo(data.get_body());
        //设置.addlog("1");
        for (size_t i = 0; i < data.get_assets().size(); i++)
        {
            if (!data.get_assets()[i].get_browser_download_url()._Equal("https://gitee.com/jiangyi0923/gw2chajianfile/repository/archive/0629"))
            {
                //设置.addlog("2");
                std::string gre = *data.get_assets()[i].get_name().get();
                //设置.addlog(gre);
                //设置.addlog("3");
                for (size_t t = 0; t < 设置.全部插件数据.size(); t++)
                {

                    if (gre._Equal(设置.全部插件数据[t].Internal_name))
                    {
                        //设置.addlog("4");
                        设置.全部插件数据[t].Download_url = data.get_assets()[i].get_browser_download_url();
                        //设置.addlog(设置.全部插件数据[t].Download_url);
                    }
                }
            }
        }
        设置.addlog("解析API信息数据方法完成!");
    }
    catch (const std::exception &e)
    {
        //_M_MESSAGES(e.what());
        // mesgs << e.what();
        设置.addlog(e.what());
        设置.apierro = 301;
        设置.addlog("解析API信息数据方法失败!");
        return;
        //::cerr << e.what() << '\n';
    }
}