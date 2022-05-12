#include "settings.h"
#include <Windows.h>

#include <io.h>
#include <direct.h>
#define PATH_DELIMITER '\\'

settings::settings()
{

    pg_ini.SetUnicode(true);
    全部插件数据.push_back({0, "gw2addon_arcdps.dll", "ARCDPS插件", "", "Shift + Alt + T 设置界面\r\nShift + Alt + H 隐藏 / 显示所有界面\r\nShift + Alt + B BUFF面板（BUFF TABLE）显示/隐藏\r\nShift + Alt + C 团队统计（AREA STATS） 显示 / 隐藏\r\nShift + Alt + S 个人统计（SELF） 显示 / 隐藏", true, 0, 0, ""});
    全部插件数据.push_back({1, "d3d9_arcdps_sct.dll", "SCT流动输出", "", "快捷键:无,设置在dps插件设置菜单内\r\n不太稳定,不建议安装,在战场和pvp概率报错", false, 0, 0, ""});
    全部插件数据.push_back({2, "d3d9_arcdps_mechanicschs.dll", "团队机制插件", "", "日志快捷键:Alt+Shift+L\r\n文本快捷键:Alt+Shift+N\r\n团队插件不打10人本不建议安装", false, 0, 0, ""});
    全部插件数据.push_back({3, "d3d9_arcdps_tablechs.dll", "团队增益插件", "", "快捷键:Alt+Shift+B\r\n团队插件不打10人本不建议安装", false, 0, 0, ""});
    全部插件数据.push_back({4, "arcdps_healing_stats.dll", "治疗统计插件", "", "快捷键:无,设置在dps插件设置菜单内\r\n团队插件不打10人本不建议安装", false, 0, 0, ""});
    全部插件数据.push_back({5, "d3d9_arcdps_buildpad.dll", "配装板插件", "", "快捷键:Alt+Shift+D\r\n会访问激战2WIKI获取相关数据\r\n可能会连接失败并提示", false, 0, 0, ""});
    全部插件数据.push_back({6, "d3d9_arcdps_SY_CN_Tool.dll", "神油工具插件", "", "设置在dps插件设置菜单内\r\nF8 boss计时器和网页日常界面\r\nF9 鼠标跟随方块\r\nF10 挤线器界面\r\n以上按键可在设置界面更改\r\n如果启用总是显示快捷键将会无效", true, 0, 0, ""});
    全部插件数据.push_back({7, "d3d9_arcdps_MountTool.dll", "神油坐骑插件", "", "快捷键:Alt+Shift+M,设置在dps插件设置菜单内", true, 0, 0, ""});
    全部插件数据.push_back({8, "d3d9_arcdps_sytool.dll", "配方搜索插件", "", "快捷键:Alt+Shift+P", false, 0, 0, ""});

    全部插件数据.push_back({9, "d912pxy\\gw2addon_d912pxy.dll", "D912pxy插件", "", "快捷键:无\r\n请勾选配置:\r\n4+16G:适用于4G显存16G内存电脑\r\n8+16G:适用于8G显存16G内存电脑\r\n低配置:适用于其他低配电脑", false, 0, 0, ""});
    全部插件数据.push_back({10, "gw2addon_ReShade64.dll", "ReShade插件", "", "快捷键:HOME\r\n效果配置文件名:DefaultPreset.ini \r\n注意: 要是启用了不兼容的模块导致游戏闪退 \r\n请删除 DefaultPreset.ini 再次进游戏即可恢复\r\n可实时自定义效果,自带本人调试的基础效果不喜可以自己调试", false, 0, 0, ""});
    全部插件数据.push_back({11, "d3d9.dll", "SweetFX插件", "", "开关快捷键:INSERT\r\n效果配置过于繁琐而且不方便查看适合懒人,不能与reshard滤镜,d912pxy共存", false, 0, 0, ""});
    全部插件数据.push_back({12, "gw2addon_ReShade64.dll", "GShade插件", "", "本插件为ReShade插件的改版插件\r\n快捷键:HOME\r\n效果配置文件名:gshade-presets/Custom/Off.ini \r\n注意: 要是启用了不兼容的模块导致游戏闪退 \r\n请删除 Off.ini 再次进游戏即可恢复\r\n可实时自定义效果,自带本人调试的基础效果不喜可以自己调试", false, 0, 0, ""});

    全部插件数据.push_back({96, "d3d9_chainload.dll", "调用文件", "", "", false, 0, 0, ""});                           // 13
    全部插件数据.push_back({97, "d3d9.dll", "WG专用调用文件", "", "", false, 0, 0, ""});                               // 14
    全部插件数据.push_back({98, "d3d9_wrapper\\gw2addon_d3d9_wrapper.dll", "ARCDPS插件包装", "", "", true, 0, 0, ""}); // 15
    全部插件数据.push_back({99, "addonLoader.dll", "配置文件", "", "", true, 0, 0, ""});                               // 16
}

settings::~settings()
{
}

float settings::项目进度()
{
    return 项目完成数量 / 项目总计数量;
}

string settings::GetFIleDescription(CHAR *file_path)
{
    string description = "";

    //获取版本信息大小
    DWORD dwSize = GetFileVersionInfoSizeA(file_path, nullptr);
    if (dwSize > 0)
    {
        CHAR *pBuf = new CHAR[dwSize + 1];
        memset(pBuf, 0, dwSize + 1);
        //获取版本信息
        GetFileVersionInfoA(file_path, NULL, dwSize, pBuf);

        // Read the list of languages and code pages.
        LPVOID lpBuffer = nullptr;
        UINT uLen = 0;

        UINT nQuerySize;
        DWORD *pTransTable;
        ::VerQueryValueA(pBuf, "\\VarFileInfo\\Translation", (void **)&pTransTable, &nQuerySize);
        DWORD m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));

        CHAR SubBlock[50] = {0};
        sprintf_s(SubBlock, 50, R"(\StringFileInfo\%08lx\ProductVersion)", m_dwLangCharset);
        // CompanyName
        //     FileDescription
        //     FileVersion
        //     InternalName
        //     LegalCopyright
        //     OriginalFilename
        //     ProductName
        //     ProductVersion
        //     Comments
        //     LegalTrademarks
        //     PrivateBuild
        //     SpecialBuild
        VerQueryValueA(pBuf, SubBlock, &lpBuffer, &uLen);
        if (uLen)
            description = (CHAR *)lpBuffer;
        delete[] pBuf;
    }
    printf("%s \r\n", description.c_str());
    string description1 = description.substr(0, 2);
    string description2 = description.substr(3, 2);
    v1 = std::stoi(description1);
    v2 = std::stoi(description2);
    //   printf("%s \r\n", description1.c_str());
    //    printf("%s \r\n", description2.c_str());
    addlog("当前电脑 vc++ 版本:");
    addlog(description);
    return description;
}

void settings::Versionchick()
{

    GetFIleDescription(R"(C:\Windows\System32\vcomp140.dll)");
    if (v1 >= 14 && v2 >= 27)
    {
        addlog("VC++版本合格");
    }
    else
    {
        v1 = 0;
        v2 = 0;
        GetFIleDescription(R"(C:\Windows\System32\vcamp140.dll)");
        if (v1 >= 14 && v2 >= 27)
        {
            addlog("VC++版本合格");
        }
        else
        {
            v1 = 0;
            v2 = 0;
            GetFIleDescription(R"(C:\Windows\System32\vcruntime140.dll)");
            if (v1 >= 14 && v2 >= 27)
            {
                addlog("VC++版本合格");
            }
            else
            {
                v1 = 0;
                v2 = 0;
                GetFIleDescription(R"(C:\Windows\SysWOW64\vcomp140.dll)");
                if (v1 >= 14 && v2 >= 27)
                {
                    addlog("VC++版本合格");
                }
                else
                {
                    v1 = 0;
                    v2 = 0;
                    GetFIleDescription(R"(C:\Windows\SysWOW64\vcamp140.dll)");
                    if (v1 >= 14 && v2 >= 27)
                    {
                        addlog("VC++版本合格");
                    }
                    else
                    {
                        v1 = 0;
                        v2 = 0;
                        GetFIleDescription(R"(C:\Windows\SysWOW64\vcruntime140.dll)");
                        if (v1 >= 14 && v2 >= 27)
                        {
                            addlog("VC++版本合格");
                        }
                        else
                        {
                            vc版本不合格 = true;
                            addlog("VC++版本不合格!请安装v14.27以上vc++2015-2019或vc++2015-2022!");
                        }
                    }
                }
            }
        }
    }
}

void settings::load_set()
{
    addlog("开始读取设置存档");
    // CreateDir(GetExePath() + "\\Installcache");
    pg_ini.SetUnicode(true);
    //游戏根目录 = GetExePath();
    std::string ppp = 游戏根目录 + "\\Installcache\\install.ini";
    pg_ini.LoadFile(ppp.c_str());
    dx11模式 = std::stoi(pg_ini.GetValue("general", "dx11模式", "0"));
    d912pxy_配置 = std::stoi(pg_ini.GetValue("general", "d912pxy_配置", "2"));
    字体大小 = (float)pg_ini.GetDoubleValue("general", "字体大小", 18.0);
    主题样式 = std::stoi(pg_ini.GetValue("general", "主题样式", "0"));
    首次使用 = pg_ini.GetBoolValue("general", "首次使用", true);
    for (size_t i = 0; i < 全部插件数据.size(); i++)
    {
        全部插件数据[i].used = pg_ini.GetBoolValue("general", 全部插件数据[i].display_name.c_str(), 全部插件数据[i].used);
    }

    pg_ini_dps.SetUnicode(true);
    std::string ppp2 = 游戏根目录 + "\\addons\\arcdps\\arcdps.ini";
    pg_ini_dps.LoadFile(ppp2.c_str());
    DPS字体大小 = std::stoi(pg_ini_dps.GetValue("session", "font_size", "13"));

    addlog("已成功读取/设定选项存档");
}

void settings::save_set()
{
    addlog("开始保存设置存档");
    pg_ini.SetUnicode(true);
    pg_ini.SetValue("general", "dx11模式", std::to_string(dx11模式).c_str());
    pg_ini.SetValue("general", "d912pxy_配置", std::to_string(d912pxy_配置).c_str());
    pg_ini.SetBoolValue("general", "首次使用", 首次使用);
    pg_ini.SetDoubleValue("general", "字体大小", (double)字体大小);
    pg_ini.SetValue("general", "主题样式", std::to_string(主题样式).c_str());
    // writelog(to_string(设置.首次使用));

    for (size_t i = 0; i < 全部插件数据.size(); i++)
    {
        pg_ini.SetBoolValue("general", 全部插件数据[i].display_name.c_str(), 全部插件数据[i].used);
    }

    std::string ppp = 游戏根目录 + "\\Installcache\\install.ini";
    pg_ini.SaveFile(ppp.c_str());

    addlog("保存设置存档");
}

void settings::save_dps()
{
    pg_ini_dps.SetUnicode(true);
    pg_ini_dps.SetValue("session", "font_size", std::to_string(DPS字体大小).c_str());
    pg_ini_dps.SetValue("panel_metrics", "visible", "1");
    pg_ini_dps.SetValue("panel_metrics", "params", "AAAAAAAAAAABAAAAAAAAAOW4p+eOhzpAMSDlu7bov586QDIg5ZON5bqUOkAzAAAA");
    std::string ppp = 游戏根目录 + "\\addons\\arcdps\\arcdps.ini";
    pg_ini_dps.SaveFile(ppp.c_str());
}

string settings::GetExePath(void)
{
    char szFilePath[MAX_PATH + 1] = {0};
    GetModuleFileNameA(nullptr, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
    string path = szFilePath;
    return path;
}

//得到文件是否存在
bool settings::file_exists(string name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

//删除目录
bool settings::RemoveDir(string FileDir)
{
    string strDir = FileDir;
    if (strDir.at(strDir.length() - 1) != '\\')
        strDir += '\\';
    WIN32_FIND_DATAA wfd;
    HANDLE hFind = FindFirstFileA((strDir + "*.*").c_str(), &wfd);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;
    do
    {
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (_stricmp(wfd.cFileName, ".") != 0 &&
                _stricmp(wfd.cFileName, "..") != 0)
                RemoveDir((strDir + wfd.cFileName).c_str());
        }
        else
        {
            DeleteFileA((strDir + wfd.cFileName).c_str());
        }
    } while (FindNextFileA(hFind, &wfd));
    FindClose(hFind);
    RemoveDirectoryA(FileDir.c_str());
    //输出内容 += "删除目录 " + szFileDir + "\r\n";
    return true;
}
//创建目录文件夹
void settings::CreateDir(string FileDir)
{
    std::string folder_builder;
    std::string sub;
    sub.reserve(FileDir.size());
    for (auto it = FileDir.begin(); it != FileDir.end(); ++it)
    {
        // cout << *(folder.end()-1) << endl;
        const char c = *it;
        sub.push_back(c);
        if (c == PATH_DELIMITER || it == FileDir.end() - 1)
        {
            folder_builder.append(sub);
            if (0 != ::_access(folder_builder.c_str(), 0))
            {
                // this folder not exist
                if (0 != ::_mkdir(folder_builder.c_str()))
                {
                    // create failed
                    return;
                }
            }
            sub.clear();
        }
    }
}

//目录名称检测
bool settings::mincjianc()
{
    bool trr = false;

    int i = 0;
    while (游戏根目录[i])
    {
        if (((unsigned char)游戏根目录[i]) > 128)
        {
            trr = true;
            break;
            i += 2;
        }
        else
        {
            i++;
        }
    }
    return trr;
}

//删除文件
void settings::file_delete(string filePath)
{
    if (file_exists(filePath))
    {
        if (remove(filePath.c_str()) == 0)
        {
            addlog("删除" + filePath + "成功");
        }
        else
        {
            addlog("删除" + filePath + "失败");
        }
    }
}
//重命名
void settings::file_rename(string filePath, string tofilePath)
{
    if (file_exists(filePath) && !file_exists(tofilePath))
    {
        if (rename(filePath.c_str(), tofilePath.c_str()) == 0)
        {
        }
    }
}
//复制文件
void settings::file_copy_to(string filePath, string tofilePath)
{
    if (file_exists(filePath))
    {
        CopyFileA(filePath.c_str(), tofilePath.c_str(), true);
    }
}

void settings::addlog(string mesgs)
{
    output.append(mesgs + "\r\n");
}

void settings::mesgebox(string pszTitle, string pszMsg)
{
    int bufflen = MultiByteToWideChar(CP_UTF8, 0, pszMsg.c_str(), -1, nullptr, 0);
    WCHAR *wideMsg = new WCHAR[bufflen + 1];
    ::memset(wideMsg, 0, sizeof(WCHAR) * (bufflen + 1));
    MultiByteToWideChar(CP_UTF8, 0, pszMsg.c_str(), -1, wideMsg, bufflen);

    bufflen = MultiByteToWideChar(CP_UTF8, 0, pszTitle.c_str(), -1, nullptr, 0);
    WCHAR *wideTitle = new WCHAR[bufflen + 1];
    ::memset(wideTitle, 0, sizeof(WCHAR) * (bufflen + 1));
    MultiByteToWideChar(CP_UTF8, 0, pszTitle.c_str(), -1, wideTitle, bufflen);
    MessageBoxW(NULL, wideMsg, wideTitle, 0);
    delete[] wideMsg;
    delete[] wideTitle;
}