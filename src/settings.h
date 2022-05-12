#pragma once
#include <string>
#include <vector>
#include "SimpleIni.h"
using namespace std;

class settings
{
public:
    struct 插件
    {
        int id;
        string file_check_name = "";
        string display_name = "";
        string Internal_name = "";
        string helpstring = "";
        bool used = false;
        int Download_size = 0;
        int file_size = 1;
        char Down_buf[64] = "";
        string file_md5 = "";
        float dowpr = 0.0f;
        bool is_inDownload = false;
        bool is_Download_ok = false;
        bool is_Download_erro = false;
        string Download_url = "";
    };
    settings();
    ~settings();
    void mesgebox(string pszTitle, string pszMsg);
    vector<插件> 全部插件数据;
    void addlog(string mesgs);
    string output;
    int y, m, d;
    string dmd5s;
    string fileday = "未获取到/获取中";
    int apierro = 0;
    bool 限制安装 = false;
    int dx11模式 = 0;
    bool 首次使用 = true;
    bool 输入正确 = false;
    bool isStarttodo = false;
    float 字体大小;
    int 主题样式 = 0;
    string 游戏根目录;
    bool 空中网客户端;
    int d912pxy_配置 = 2;
    bool vc版本不合格 = false;
    //得到当前程序目录
    string GetExePath(void);
    //创建目录文件夹
    void CreateDir(string FileDir);
    //删除目录
    bool RemoveDir(string FileDir);
    //目录名称检测
    bool mincjianc();
    //得到文件是否存在
    bool file_exists(string name);
    //复制文件
    void file_copy_to(string filePath, string tofilePath);
    //重命名文件
    void file_rename(string filePath, string tofilePath);
    //删除文件
    void file_delete(string filePath);

    void load_set();
    void save_set();
    void Versionchick();
    float 项目完成数量 = 0;
    float 项目总计数量 = 0;
    float 项目进度();
    int DPS字体大小 = 13;
    void save_dps();
private:
    CSimpleIniA pg_ini;
    CSimpleIniA pg_ini_dps;
    string GetFIleDescription(CHAR *file_path);
    int v1, v2;
};
