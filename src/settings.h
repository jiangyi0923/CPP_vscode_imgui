#pragma once
#include <string>
#include <vector>
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
    vector<插件> 全部插件数据;
    void addlog(string mesgs);
    string output;
private:
    
};
