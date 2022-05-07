#include "settings.h"

settings::settings()
{
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
    全部插件数据.push_back({10, "ReShade64\\gw2addon_ReShade64.dll", "ReShade插件", "", "快捷键:HOME\r\n效果配置文件名:DefaultPreset.ini \r\n注意: 要是启用了不兼容的模块导致游戏闪退 \r\n请删除 DefaultPreset.ini 再次进游戏即可恢复\r\n可实时自定义效果,自带本人调试的基础效果不喜可以自己调试", false, 0, 0, ""});
    全部插件数据.push_back({11, "d3d9.dll", "SweetFX插件", "", "开关快捷键:INSERT\r\n效果配置过于繁琐而且不方便查看适合懒人,不能与reshard滤镜,d912pxy共存", false, 0, 0, ""});
    全部插件数据.push_back({96, "d3d9_chainload.dll", "调用文件", "", "", false, 0, 0, ""});                           // 12
    全部插件数据.push_back({97, "d3d9.dll", "WG专用调用文件", "", "", false, 0, 0, ""});                               // 13
    全部插件数据.push_back({98, "d3d9_wrapper\\gw2addon_d3d9_wrapper.dll", "ARCDPS插件包装", "", "", true, 0, 0, ""}); // 14
    全部插件数据.push_back({99, "addonLoader.dll", "配置文件", "", "", true, 0, 0, ""});                               // 15
}

settings::~settings()
{

}

void settings::addlog(string mesgs)
{
    output.append(mesgs + "\r\n");
}