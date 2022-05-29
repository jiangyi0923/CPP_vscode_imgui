
#include "install.h"

#include <wininet.h>
#include <thread>
#pragma comment(lib, "wininet.lib")

#include "MyMd5.h"
#include "unzip.h"

install::install(/* args */)
{
	//设置.addlog("开始安装");
}

install::~install()
{
}

bool install::md5chick2(string path, string md5v)
{
	string md5 = MD5_file(path.c_str(), 32);
	bool tmpmd5 = md5v._Equal(md5);
	设置.write_log("文件的md5是:" + md5 + "  实际的md5应是:" + md5v + "  是否相等:" + to_string(tmpmd5));
	return tmpmd5;
}
bool install::unzipfs_ss(string FileDir, string outFileDir)
{
	if (!设置.file_exists(FileDir))
	{
		设置.mesgebox("解压文件失败", FileDir + "文件不存在!\r\n可能被你的杀软拦截或删除");
		设置.addlog("解压失败" + FileDir + ",文件不存在!");
		设置.write_log("解压失败" + FileDir + ",文件不存在!");
		return false;
	}

	bool wanc = true;
	SetCurrentDirectoryA(outFileDir.c_str());
	HZIP hz = OpenZip((TCHAR *)FileDir.c_str(), nullptr);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	for (int i = 0; i < numitems; i++)
	{
		GetZipItem(hz, i, &ze);
		if (UnzipItem(hz, i, ze.name) != ZR_OK)
		{
			设置.mesgebox("解压文件失败", string((char *)ze.name) + "解压失败");
			设置.addlog("解压" + string((char *)ze.name) + "失败");
			设置.write_log("解压" + string((char *)ze.name) + "失败");
			wanc = false;
			break;
		}
	}
	CloseZip(hz);
	SetCurrentDirectoryA(设置.游戏根目录.c_str());
	return wanc;
}
//检测dx12插件时候一致
bool install::chickdxneid()
{
	bool 返回值 = true;

	string dx12chajmin = 设置.游戏根目录 + "\\d912pxy\\dll\\release\\d3d9.dll";
	if (设置.全部插件数据[0].used)
	{
		dx12chajmin = 设置.游戏根目录 + "\\addons\\d912pxy\\dll\\release\\d3d9.dll";
	}
	if (设置.file_exists(dx12chajmin))
	{
		if (md5chick2(dx12chajmin, 设置.dmd5s))
		{
			string pszMsg = "发现已存在的同版本D912PXY插件文件夹,是否卸载重装?\r\n选是-删除所有dx12插件文件\r\n选否-保留所有dx12插件文件\r\n此选项作用是避免更新其他插件时重复安装此插件\r\n造成进入游戏后没必要的重新预加载游戏模型";
			string pszTitle = "是否重装D912PXY插件?";

			int bufflen = MultiByteToWideChar(CP_UTF8, 0, pszMsg.c_str(), -1, NULL, 0);
			WCHAR *wideMsg = new WCHAR[bufflen + 1];
			::memset(wideMsg, 0, sizeof(WCHAR) * (bufflen + 1));
			MultiByteToWideChar(CP_UTF8, 0, pszMsg.c_str(), -1, wideMsg, bufflen);

			bufflen = MultiByteToWideChar(CP_UTF8, 0, pszTitle.c_str(), -1, NULL, 0);
			WCHAR *wideTitle = new WCHAR[bufflen + 1];
			::memset(wideTitle, 0, sizeof(WCHAR) * (bufflen + 1));
			MultiByteToWideChar(CP_UTF8, 0, pszTitle.c_str(), -1, wideTitle, bufflen);
			// MessageBoxW(NULL, wideMsg, wideTitle, 0);
			设置.addlog("发现已存在的同版本D912PXY插件");
			if (MessageBoxW(NULL, wideMsg, wideTitle, MB_YESNO) == 6)
			{
				string didi4 = 设置.游戏根目录 + "\\d912pxy";
				if (GetFileAttributesA(didi4.c_str()))
				{
					设置.RemoveDir(didi4);
				}
				didi4 = 设置.游戏根目录 + "\\addons\\d912pxy";
				if (GetFileAttributesA(didi4.c_str()))
				{
					设置.RemoveDir(didi4);
				}
			}
			else
			{
				// copyfilesto(dx12chajmin, 游戏根目录 + "\\bin64\\d912pxy.dll");
				返回值 = false;
			}
			delete[] wideMsg;
			delete[] wideTitle;
		}
		else
		{
			string didi4 = 设置.游戏根目录 + "\\d912pxy";
			if (GetFileAttributesA(didi4.c_str()))
			{
				设置.RemoveDir(didi4);
			}
		}
	}
	return 返回值;
}

void install::download_S(settings::插件 *插件插件)
{
	插件插件->is_Download_erro = false;
	插件插件->dowpr = 0.0f;
	插件插件->Download_size = 0;
	插件插件->is_Download_ok = false;
	try
	{
		BYTE Temp[1024];
		ULONG Number = 1;
		FILE *stream;
		HINTERNET hSession = InternetOpenA("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hSession != NULL)
		{
			设置.write_log("hSession创建成功");
			设置.addlog(插件插件->Download_url.c_str());
			// printf(插件插件.Download_url.c_str());
			HINTERNET handle2 = InternetOpenUrlA(hSession, 插件插件->Download_url.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
			if (handle2 != NULL)
			{
				设置.write_log("handle2创建成功");
				int downloadNow = 0;
				string save_as = 设置.GetExePath() + "\\Installcache\\" + 插件插件->Internal_name;
				设置.write_log("开始创建并写入" + save_as);
				if (fopen_s(&stream, save_as.c_str(), "wb") != EINVAL)
				{
					while (Number > 0)
					{
						InternetReadFile(handle2, Temp, 1024 - 1, &Number);
						// printf_s(to_string(Number).c_str());
						// fewtext += to_string(Number + 1) + "\r\n";
						downloadNow += Number;
						插件插件->dowpr = (float)downloadNow / (float)插件插件->file_size;
						插件插件->Download_size = (int)downloadNow;

						fwrite(Temp, sizeof(char), Number, stream);
					}
					fclose(stream);
					
				}

				InternetCloseHandle(handle2);
				handle2 = NULL;
			}
			InternetCloseHandle(hSession);
			hSession = NULL;
			插件插件->is_inDownload = false;
			插件插件->is_Download_ok = true;
			设置.write_log("下载完成");
		}
	}
	catch (const std::exception &e)
	{
		// std::cerr << e.what() << '\n';
		设置.addlog(e.what());
		插件插件->is_Download_erro = true;
		插件插件->is_Download_ok = true;
		设置.write_log("下载失败");
		设置.write_log(e.what());
	}
}

void install::download(settings::插件 *插件插件) /*将Url指向的地址的文件下载到save_as指向的本地文件*/
{
	设置.write_log("创建下载线程");
	std::thread cprCall(&install::download_S, this, 插件插件);
	cprCall.detach();
}

void install::冲突检测()
{
	string bin64 = 设置.游戏根目录 + "\\bin64";
	vector<string> 所有文件名{
		"d3d9.dll",
		"d3d9.dll.md5sum",
		"d3d9_arcdps_buildtemplates.dll",
		"d3d9_arcdps_extras.dll",
		"d3d9_arcdps_mechanicschs.dll",
		"d3d9_arcdps_mechanics.dll",
		"d3d9_arcdps_buildpad.dll",
		"d3d9_arcdps_sytool.dll",
		"d3d9_arcdps_ct.dll",
		"d3d9_arcdps_noex.dll",
		"d3d9_chainload.dll",
		"d3d9_chainload_noex.dll",
		"d3d9_arcdps_tablechs.dll",
		"d3d9_arcdps_table.dll",
		"d3d9_arcdps_sct.dll",
		"arcdps_healing_stats.dll",
		"ReShade64.dll",
		"ReShade.ini",
		"DefaultPreset.ini",
		"d3d9_mchain.dll",
		"SweetFX readme.txt",
		"SweetFX_preset.txt",
		"SweetFX_settings.txt",
		"dxgi.dll",
		"d3d9_ReShade641.zip",
		"d3d9_arcdps_MountTool.dll",
		"d3d9_arcdps_SY_CN_Tool.dll",
		"d912pxy.dll",
		"SweetFX.zip",
		"ReShade.fx",
		"Sweet.fx",
		"dxgi.log",
		"d3d9_mchain.log",
		"ReShade64.log",
		"log.log",
		"gw2addon_arcdps.dll",
		"addonLoader.dll",
		"d3d11.dll",
		"gw2addon_ReShade64.dll",
		"GShade.ini",
	};

	设置.addlog("开始清理文件");
	for (size_t i = 0; i < 所有文件名.size(); i++)
	{
		设置.file_delete(bin64 + "\\" + 所有文件名[i]);
		设置.file_delete(设置.游戏根目录 + "\\" + 所有文件名[i]);
		设置.file_delete(设置.游戏根目录 + R"(\addons\arcdps\)" + 所有文件名[i]);
	}
	Sleep(1000);

	if (GetFileAttributesA((设置.游戏根目录 + "\\addons\\d3d9_wrapper").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\addons\\d3d9_wrapper");
	}
	if (GetFileAttributesA((设置.游戏根目录 + "\\addons\\ReShade64").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\addons\\ReShade64");
	}
	if (GetFileAttributesA((设置.游戏根目录 + "\\SweetFX").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\SweetFX");
	}
	if (GetFileAttributesA((bin64 + "\\SweetFX").c_str()))
	{
		设置.RemoveDir(bin64 + "\\SweetFX");
	}
	if (GetFileAttributesA((设置.游戏根目录 + "\\reshade-shaders").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\reshade-shaders");
	}
	if (GetFileAttributesA((bin64 + "\\reshade-shaders").c_str()))
	{
		设置.RemoveDir(bin64 + "\\reshade-shaders");
	}

	if (GetFileAttributesA((设置.游戏根目录 + "\\gshade-shaders").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\gshade-shaders");
	}
	if (GetFileAttributesA((bin64 + "\\gshade-shaders").c_str()))
	{
		设置.RemoveDir(bin64 + "\\gshade-shaders");
	}

	if (GetFileAttributesA((设置.游戏根目录 + "\\gshade-presets").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\gshade-presets");
	}
	if (GetFileAttributesA((bin64 + "\\gshade-presets").c_str()))
	{
		设置.RemoveDir(bin64 + "\\gshade-presets");
	}

	if (是卸载)
	{
		if (GetFileAttributesA((设置.游戏根目录 + "\\d912pxy").c_str()))
		{
			设置.RemoveDir(设置.游戏根目录 + "\\d912pxy");
		}
		if (GetFileAttributesA((设置.游戏根目录 + "\\addons\\d912pxy").c_str()))
		{
			设置.RemoveDir(设置.游戏根目录 + "\\addons\\d912pxy");
		}
	}
	else
	{
	}
}

void install::卸载逻辑_线程()
{
	冲突检测();
	if (GetFileAttributesA((设置.游戏根目录 + "\\addons").c_str()))
	{
		设置.RemoveDir(设置.游戏根目录 + "\\addons");
	}
	设置.mesgebox("卸载完毕!", "您现在可以关闭程序或者重新安装插件!");
}

void install::卸载逻辑()
{
	std::thread cprCall([this]()
						{ 卸载逻辑_线程(); });
	cprCall.detach();
}

void install::安装逻辑()
{
	if (是疑难安装)
	{
		设置.write_log("疑难安装");
		std::thread cprCall([this]()
							{ 安装逻辑_线程_疑难(); });
		cprCall.detach();
	}
	else
	{
		设置.write_log("正常安装");
		std::thread cprCall([this]()
							{ 安装逻辑_线程(); });
		cprCall.detach();
	}
}

void install::安装逻辑_线程_疑难()
{
	冲突检测();

	设置.CreateDir(设置.游戏根目录 + "\\bin64");
	设置.CreateDir(设置.游戏根目录 + "\\addons");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\arcdps");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\sct");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\sct\\fonts");
	设置.addlog("创建临时目录");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\peizi");

	string 存放目录 = 设置.游戏根目录 + "\\Installcache\\";
	string addons = 设置.游戏根目录 + "\\addons";
	string bin64 = 设置.游戏根目录 + "\\bin64";
	string arcdps = 设置.游戏根目录 + "\\addons\\arcdps";
	string peizi = 设置.游戏根目录 + "\\addons\\peizi";

	int 错误计数 = 0;

	if (!unzipfs_ss(存放目录 + "peizi.zip", 设置.游戏根目录 + "\\addons\\peizi"))
	{
		设置.addlog("配置压缩文件解压失败!");
	}
	vector<string> 文件名1{
		R"(\addons\arcdps\arcdps_font.ttf)",
		R"(\addons\arcdps\arcdps_lang.ini)",
		R"(\addons\arcdps\arcdps_table_lang.ini)",
		R"(\addons\sct\fonts\fanzhengmiaowu.ttf)",
		R"(\addons\sct\lang.ini)",
		R"(\addons\sct\sct.ini)",
		R"(\addons\sct\lang.json)",
		R"(\addons\sct\sct.json)",
	};
	vector<string> 文件名2{
		R"(\addons\peizi\arcdps_font.ttf)",
		R"(\addons\peizi\arcdps_lang.ini)",
		R"(\addons\peizi\arcdps_table_lang.ini)",
		R"(\addons\peizi\fanzhengmiaowu.ttf)",
		R"(\addons\peizi\lang.ini)",
		R"(\addons\peizi\sct.ini)",
		R"(\addons\peizi\lang.json)",
		R"(\addons\peizi\sct.json)",
	};

	for (size_t i = 0; i < 文件名1.size(); i++)
	{
		if (!设置.file_exists(设置.游戏根目录 + 文件名1[i]))
		{
			设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
		}
		else
		{
			if (文件名1[i].find("arcdps_lang.ini", 1) < 200)
			{
				设置.file_delete(设置.游戏根目录 + 文件名1[i]);
				设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
			}
			else if (文件名1[i].find("arcdps_table_lang.ini", 1) < 200)
			{
				设置.file_delete(设置.游戏根目录 + 文件名1[i]);
				设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
			}
			else if (文件名1[i].find("lang.json", 1) < 200)
			{
				设置.file_delete(设置.游戏根目录 + 文件名1[i]);
				设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
			}
		}
	}
	if (设置.file_exists(设置.游戏根目录 + 文件名1[5]))
	{
		设置.file_delete(设置.游戏根目录 + 文件名1[5]);
	}

	设置.addlog("布置配置和字体文件完成");
	设置.项目完成数量++;

	if (设置.全部插件数据[0].used)
	{
		if (!unzipfs_ss(存放目录 + 设置.全部插件数据[0].Internal_name, 设置.游戏根目录))
		{
			设置.addlog(设置.全部插件数据[0].display_name + "压缩文件解压失败!");
		}
		Sleep(200);
		int 计次 = 0;
		while (!设置.file_exists(设置.游戏根目录 + "\\d3d11.dll") && 计次 < 20)
		{

			if (设置.file_exists(设置.游戏根目录 + "\\d3d9.dll"))
			{
				设置.file_rename(设置.游戏根目录 + "\\d3d9.dll", 设置.游戏根目录 + "\\d3d11.dll");
			}
			Sleep(100);
			计次++;
		}
		if (计次 < 20)
		{
			设置.项目完成数量++;
			设置.addlog(设置.全部插件数据[0].display_name + "安装完成");
		}
		else
		{
			设置.addlog(设置.全部插件数据[0].display_name + "安装失败!请重试");
			错误计数++;
		}
	}

	for (size_t i = 1; i < 9; i++)
	{
		if (设置.全部插件数据[i].used)
		{
			if (!unzipfs_ss(存放目录 + 设置.全部插件数据[i].Internal_name, bin64))
			{
				设置.addlog(设置.全部插件数据[i].display_name + "压缩文件解压失败!");
			}
			Sleep(200);
			int 计次 = 0;
			while (!设置.file_exists(bin64 + "\\" + 设置.全部插件数据[i].file_check_name) && 计次 < 20)
			{
				Sleep(100);
				计次++;
			}
			if (计次 < 20)
			{
				设置.项目完成数量++;
				设置.addlog(设置.全部插件数据[i].display_name + "安装完成");
			}
			else
			{
				设置.addlog(设置.全部插件数据[i].display_name + "安装失败!请重试");
				错误计数++;
			}
		}
	}

	if (设置.全部插件数据[10].used)
	{
		if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, 设置.游戏根目录))
		{
			设置.addlog("解压ReShade64.zip失败! 请重试!");
			错误计数++;
		}
		Sleep(200);
		int 计次 = 0;
		while (!设置.file_exists(设置.游戏根目录 + "\\dxgi.dll") && 计次 < 20)
		{

			if (设置.file_exists(设置.游戏根目录 + "\\gw2addon_ReShade64.dll"))
			{
				设置.file_rename(设置.游戏根目录 + "\\gw2addon_ReShade64.dll", 设置.游戏根目录 + "\\dxgi.dll");
			}
			Sleep(100);
			计次++;
		}
		if (计次 < 20)
		{
			设置.项目完成数量++;
			设置.addlog(设置.全部插件数据[10].display_name + "安装完成");
		}
		else
		{
			设置.addlog(设置.全部插件数据[10].display_name + "安装失败!请重试");
			错误计数++;
		}
	}
	else if (设置.全部插件数据[12].used)
	{
		// GShade
		bool repugs = false;
		string reshard目录 = 设置.游戏根目录 + "\\addons\\ReShade64";
		设置.CreateDir(reshard目录);
		if (!unzipfs_ss(存放目录 + 设置.全部插件数据[12].Internal_name, reshard目录))
		{
			设置.addlog("解压GShade.zip失败! 请重试!");
			错误计数++;
		}
		int 计次 = 0;
		while (!设置.file_exists(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name) && 计次 < 20)
		{
			Sleep(100);
			计次++;
		}
		if (计次 < 20)
		{
			// if (!设置.全部插件数据[0].used)
			{
				设置.file_copy_to(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name, 设置.游戏根目录 + "\\dxgi.dll");
			}
			设置.file_copy_to(reshard目录 + "\\GShade.ini", 设置.游戏根目录 + "\\GShade.ini");
			repugs = true;
		}
		if (repugs)
		{
			设置.addlog("GShade滤镜插件已安装-dx11模式");
			设置.项目完成数量++;
		}
		else
		{
			设置.addlog("GShade滤镜插件安装失败! 请重试!");
			错误计数++;
		}
	}

	设置.addlog("删除临时目录");
	设置.RemoveDir(设置.游戏根目录 + "\\addons\\peizi");
	设置.RemoveDir(设置.游戏根目录 + "\\addons\\chajian");

	if (错误计数 > 0)
	{
		string 输出内容 = "安装失败!失败项目数:" + std::to_string(错误计数) + "\r\n您需要尝试重新点击安装!";
		设置.mesgebox("安装失败!", 输出内容);
		设置.addlog(输出内容);
	}
	else
	{
		设置.mesgebox("安装完成!", "请关闭本程序并正常启动游戏\r\n如有问题请前往QQ群寻求帮助\r\nQQ群:\r\n1群:863308792\r\n2群:626045750\r\nQQ群3:1029866604");
		设置.addlog("安装完成!");
	}

	设置.isStarttodo = false;
}

void install::安装逻辑_线程()
{
	冲突检测();

	设置.CreateDir(设置.游戏根目录 + "\\bin64");
	设置.CreateDir(设置.游戏根目录 + "\\addons");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\arcdps");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\sct");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\sct\\fonts");
	设置.addlog("创建临时目录");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\peizi");
	设置.CreateDir(设置.游戏根目录 + "\\addons\\chajian");

	string 存放目录 = 设置.游戏根目录 + "\\Installcache\\";
	string addons = 设置.游戏根目录 + "\\addons";
	string bin64 = 设置.游戏根目录 + "\\bin64";
	string arcdps = 设置.游戏根目录 + "\\addons\\arcdps";
	string peizi = 设置.游戏根目录 + "\\addons\\peizi";

	设置.项目完成数量++;

	int 错误计数 = 0;

	if (!unzipfs_ss(存放目录 + "peizi.zip", 设置.游戏根目录 + "\\addons\\peizi"))
	{
		设置.addlog("配置压缩文件解压失败!");
	}

	vector<string> 文件名1{
		R"(\addons\arcdps\arcdps_font.ttf)",
		R"(\addons\arcdps\arcdps_lang.ini)",
		R"(\addons\arcdps\arcdps_table_lang.ini)",
		R"(\addons\sct\fonts\fanzhengmiaowu.ttf)",
		R"(\addons\sct\lang.ini)",
		R"(\addons\sct\sct.ini)",
		R"(\addons\sct\lang.json)",
		R"(\addons\sct\sct.json)",
	};
	vector<string> 文件名2{
		R"(\addons\peizi\arcdps_font.ttf)",
		R"(\addons\peizi\arcdps_lang.ini)",
		R"(\addons\peizi\arcdps_table_lang.ini)",
		R"(\addons\peizi\fanzhengmiaowu.ttf)",
		R"(\addons\peizi\lang.ini)",
		R"(\addons\peizi\sct.ini)",
		R"(\addons\peizi\lang.json)",
		R"(\addons\peizi\sct.json)",
	};

	for (size_t i = 0; i < 文件名1.size(); i++)
	{
		if (!设置.file_exists(设置.游戏根目录 + 文件名1[i]))
		{
			设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
		}
		else
		{
			if (文件名1[i].find("arcdps_lang.ini", 1) < 200)
			{
				设置.file_delete(设置.游戏根目录 + 文件名1[i]);
				设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
			}
			else if (文件名1[i].find("arcdps_table_lang.ini", 1) < 200)
			{
				设置.file_delete(设置.游戏根目录 + 文件名1[i]);
				设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
			}
			else if (文件名1[i].find("lang.json", 1) < 200)
			{
				设置.file_delete(设置.游戏根目录 + 文件名1[i]);
				设置.file_copy_to(设置.游戏根目录 + 文件名2[i], 设置.游戏根目录 + 文件名1[i]);
			}
		}
	}
	if (设置.file_exists(设置.游戏根目录 + 文件名1[5]))
	{
		设置.file_delete(设置.游戏根目录 + 文件名1[5]);
	}

	设置.addlog("布置配置和字体文件完成");
	设置.项目完成数量++;

	bool 是否选择dps插件 = false;
	bool 是否选择dps系列插件 = false;
	if (设置.全部插件数据[0].used)
		是否选择dps插件 = true;

	for (size_t i = 0; i < 9; i++)
	{
		if (!是否选择dps系列插件 && i != 0 && 设置.全部插件数据[i].used)
			是否选择dps系列插件 = true;
	}

	for (size_t i = 0; i < 9; i++)
	{
		if (设置.全部插件数据[i].used)
		{
			if (!unzipfs_ss(存放目录 + 设置.全部插件数据[i].Internal_name, arcdps))
			{
				设置.addlog(设置.全部插件数据[i].display_name + "压缩文件解压失败!");
			}
			Sleep(200);
			if (i == 0)
			{
				if (设置.file_exists(arcdps + "\\d3d9.dll"))
				{
					设置.file_rename(arcdps + "\\d3d9.dll", arcdps + "\\gw2addon_arcdps.dll");
				}
			}
			int 计次 = 0;
			while (!设置.file_exists(arcdps + "\\" + 设置.全部插件数据[i].file_check_name) && 计次 < 20)
			{
				Sleep(100);
				计次++;
			}
			if (计次 < 20)
			{
				设置.项目完成数量++;
				设置.addlog(设置.全部插件数据[i].display_name + "安装完成");
			}
			else
			{
				设置.addlog(设置.全部插件数据[i].display_name + "安装失败!请重试");
				错误计数++;
			}
		}
	}

	if (是否选择dps系列插件 || 是否选择dps插件)
	{
		//解压dps包装模块插件
		设置.CreateDir(设置.游戏根目录 + "\\addons\\d3d9_wrapper");
		if (!unzipfs_ss(存放目录 + 设置.全部插件数据[15].Internal_name, 设置.游戏根目录 + "\\addons\\d3d9_wrapper"))
		{
			设置.addlog("dps包装插件文件解压失败!");
		}
		Sleep(200);
		if (设置.file_exists(设置.游戏根目录 + "\\addons\\" + 设置.全部插件数据[15].file_check_name))
		{
			设置.addlog(设置.全部插件数据[15].display_name + "安装完成");
			设置.项目完成数量++;
		}
		else
		{
			设置.addlog(设置.全部插件数据[15].display_name + "安装失败");
			错误计数++;
		}
	}

	// dps系列调用插件
	if (是否选择dps插件)
	{
		if (设置.dx11模式 == 1)
		{
			// dx11模式
			设置.file_copy_to(peizi + "\\d3d11.dll", 设置.游戏根目录 + "\\d3d11.dll");
			设置.file_copy_to(peizi + "\\dxgi.dll", 设置.游戏根目录 + "\\dxgi.dll");
			设置.file_copy_to(peizi + "\\addonLoader.dll", 设置.游戏根目录 + "\\addonLoader.dll");
		}
		else
		{
			设置.file_copy_to(peizi + "\\d3d9.dll", 设置.游戏根目录 + "\\bin64\\d3d9.dll");
			if (!设置.空中网客户端)
			{
				if (!unzipfs_ss(存放目录 + 设置.全部插件数据[14].Internal_name, 设置.游戏根目录))
				{
					设置.addlog("专用调用文件解压失败!");
				}
				Sleep(100);
				设置.file_rename(设置.游戏根目录 + "\\wgd3d9.dll", 设置.游戏根目录 + "\\d3d9.dll");
				Sleep(100);
			}
		}
		设置.file_copy_to(peizi + "\\addonLoader.dll", 设置.游戏根目录 + "\\addonLoader.dll");
		Sleep(200);
		if (设置.file_exists(设置.游戏根目录 + "\\addonLoader.dll"))
		{
			设置.addlog("addonLoader插件加载器安装完成");
			设置.项目完成数量++;
		}
		else
		{
			设置.addlog("addonLoader插件加载器安装失败");
			错误计数++;
		}
	}
	//其他插件

	// 设置.addlog(to_string(设置.dx11模式));
	if (设置.dx11模式 == 1)
	{
		设置.RemoveDir(设置.游戏根目录 + "\\addons\\d912pxy");
		设置.RemoveDir(设置.游戏根目录 + "\\d912pxy");
		string reshard目录 = 设置.游戏根目录 + "\\addons\\ReShade64";
		if (设置.全部插件数据[10].used)
		{
			bool repugs = false;

			if (!设置.全部插件数据[0].used)
			{
				设置.RemoveDir(reshard目录);
				设置.file_delete(设置.游戏根目录 + "\\d3d9.dll");
				设置.file_delete(设置.游戏根目录 + "\\dxgi.dll");
				设置.file_delete(设置.游戏根目录 + "\\d3d11.dll");
				设置.file_delete(设置.游戏根目录 + "\\bin64\\d3d9.dll");
				设置.file_delete(设置.游戏根目录 + "\\addonLoader.dll");

				if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, 设置.游戏根目录))
				{
					设置.addlog("解压ReShade64.zip失败! 请重试!");
					错误计数++;
				}
				Sleep(200);
				// gw2addon_
				int 计次 = 0;
				while (!设置.file_exists(设置.游戏根目录 + "\\" + 设置.全部插件数据[10].file_check_name) && 计次 < 20)
				{
					Sleep(100);
					计次++;
				}
				if (计次 < 20)
				{
					Sleep(100);
					设置.file_rename(设置.游戏根目录 + "\\gw2addon_ReShade64.dll", 设置.游戏根目录 + "\\dxgi.dll");
					Sleep(100);
					if (设置.file_exists(设置.游戏根目录 + "\\dxgi.dll"))
					{
						repugs = true;
					}
				}
			}
			else
			{
				设置.CreateDir(reshard目录);
				if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, reshard目录))
				{
					设置.addlog("解压ReShade64.zip失败! 请重试!");
					错误计数++;
				}
				Sleep(200);
				// gw2addon_
				// filerename(游戏根目录 + "\\gw2addon_ReShade64.dll", 游戏根目录 + "\\dxgi.dll");
				// Sleep(100);
				int 计次 = 0;
				while (!设置.file_exists(reshard目录 + "\\" + 设置.全部插件数据[10].file_check_name) && 计次 < 20)
				{
					Sleep(100);
					计次++;
				}
				if (计次 < 20)
				{
					repugs = true;
				}
			}

			if (repugs)
			{
				设置.addlog("ReShade滤镜插件已安装-dx11模式");
				设置.项目完成数量++;
			}
			else
			{
				设置.addlog("ReShade滤镜插件安装失败! 请重试!");
				错误计数++;
			}
		}
		else if (设置.全部插件数据[12].used)
		{
			// GShade
			bool repugs = false;
			设置.CreateDir(reshard目录);
			if (!unzipfs_ss(存放目录 + 设置.全部插件数据[12].Internal_name, reshard目录))
			{
				设置.addlog("解压GShade.zip失败! 请重试!");
				错误计数++;
			}
			int 计次 = 0;
			while (!设置.file_exists(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name) && 计次 < 20)
			{
				Sleep(100);
				计次++;
			}
			if (计次 < 20)
			{
				if (!设置.全部插件数据[0].used)
				{
					设置.file_copy_to(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name, 设置.游戏根目录 + "\\dxgi.dll");
				}
				设置.file_copy_to(reshard目录 + "\\GShade.ini", 设置.游戏根目录 + "\\GShade.ini");
				repugs = true;
			}
			if (repugs)
			{
				设置.addlog("GShade滤镜插件已安装-dx11模式");
				设置.项目完成数量++;
			}
			else
			{
				设置.addlog("GShade滤镜插件安装失败! 请重试!");
				错误计数++;
			}
		}
		else
		{
			设置.RemoveDir(reshard目录);
		}
	}
	else
	{
		string DX912_4 = peizi + "\\4_16_config.ini";
		string DX912_8 = peizi + "\\8_16_config.ini";
		string DX912_L = peizi + "\\low_config.ini";

		// d912pxy
		if (设置.全部插件数据[9].used)
		{
			bool dx12pughave = false;
			if (设置.全部插件数据[0].used)
			{
				设置.RemoveDir(设置.游戏根目录 + "\\d912pxy");
				if (chickdxneid())
				{
					// unzipfs(ID_DX912, 游戏根目录);
					if (!unzipfs_ss(存放目录 + 设置.全部插件数据[9].Internal_name, addons))
					{
						设置.addlog("解压d912pxy.zip失败! 请重试!");
						错误计数++;
					}
				}
				Sleep(200);
				if (设置.d912pxy_配置 == 0)
					设置.file_copy_to(DX912_4, addons + "\\d912pxy\\config.ini");
				if (设置.d912pxy_配置 == 1)
					设置.file_copy_to(DX912_8, addons + "\\d912pxy\\config.ini");
				if (设置.d912pxy_配置 == 2)
					设置.file_copy_to(DX912_L, addons + "\\d912pxy\\config.ini");
				if (设置.file_exists(addons + "\\d912pxy\\gw2addon_d912pxy.dll"))
				{
					dx12pughave = true;
				}
				Sleep(100);
				if (设置.file_exists(addons + "\\d912pxy\\config.ini"))
				{
					设置.addlog("D912PXY帧率插件配置文件已安装");
				}
				else
				{
					设置.addlog("D912PXY帧率插件配置文件未安装,请重试");
					错误计数++;
				}
			}
			else
			{
				设置.RemoveDir(设置.游戏根目录 + "\\addons\\d912pxy");
				// ID_DX912
				设置.file_delete(设置.游戏根目录 + "\\d3d9.dll");
				设置.file_delete(设置.游戏根目录 + "\\dxgi.dll");
				设置.file_delete(设置.游戏根目录 + "\\d3d11.dll");
				设置.file_delete(设置.游戏根目录 + "\\bin64\\d3d9.dll");
				设置.file_delete(设置.游戏根目录 + "\\addonLoader.dll");
				if (chickdxneid())
				{
					// unzipfs(ID_DX912, 游戏根目录);
					if (!unzipfs_ss(存放目录 + "\\d912pxy.zip", 设置.游戏根目录))
					{
						设置.addlog("解压d912pxy.zip失败! 请重试!");
						错误计数++;
					}
					Sleep(200);
				}
				if (设置.d912pxy_配置 == 0)
					设置.file_copy_to(DX912_4, 设置.游戏根目录 + "\\d912pxy\\config.ini");
				if (设置.d912pxy_配置 == 1)
					设置.file_copy_to(DX912_8, 设置.游戏根目录 + "\\d912pxy\\config.ini");
				if (设置.d912pxy_配置 == 2)
					设置.file_copy_to(DX912_L, 设置.游戏根目录 + "\\d912pxy\\config.ini");
				Sleep(100);
				if (设置.file_exists(设置.游戏根目录 + "\\d912pxy\\config.ini"))
				{
					设置.addlog("D912PXY帧率插件配置文件已安装");
				}
				else
				{
					设置.addlog("D912PXY帧率插件配置文件未安装,请重试");
					错误计数++;
				}

				if (!设置.空中网客户端)
				{
					if (!unzipfs_ss(存放目录 + 设置.全部插件数据[13].Internal_name, 设置.游戏根目录))
					{
						设置.addlog("专用调用文件解压失败!");
					}
					Sleep(100);
					设置.file_rename(设置.游戏根目录 + "\\wgd3d9.dll", 设置.游戏根目录 + "\\d3d9.dll");
					Sleep(100);
				}

				设置.file_copy_to(设置.游戏根目录 + R"(\d912pxy\dll\release\d3d9.dll)", 设置.游戏根目录 + "\\bin64\\d3d9.dll");
				Sleep(100);
				if (设置.file_exists(设置.游戏根目录 + "\\bin64\\d3d9.dll"))
				{
					dx12pughave = true;
				}
			}
			if (dx12pughave)
			{
				设置.addlog("D912PXY帧率插件已安装");
				设置.项目完成数量++;
			}
			else
			{
				设置.addlog("D912PXY帧率插件未安装,请重试");
				错误计数++;
			}
		}
		else
		{
			设置.RemoveDir(设置.游戏根目录 + "\\addons\\d912pxy");
			设置.RemoveDir(设置.游戏根目录 + "\\d912pxy");
		}

		string reshard目录 = 设置.游戏根目录 + "\\addons\\ReShade64";
		if (设置.全部插件数据[10].used)
		{
			bool repugs = false;
			if (设置.全部插件数据[0].used)
			{
				设置.CreateDir(reshard目录);
				if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, reshard目录))
				{
					设置.addlog("解压ReShade64.zip失败! 请重试!");
				}
				Sleep(200);
				if (设置.file_exists(reshard目录 + "\\gw2addon_ReShade64.dll"))
				{
					repugs = true;
				}
			}
			else
			{
				设置.RemoveDir(reshard目录);

				if (设置.空中网客户端)
				{
					if (设置.全部插件数据[9].used)
					{
						if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, 设置.游戏根目录))
						{
							设置.addlog("解压ReShade64.zip失败! 请重试!");
							错误计数++;
						}
						Sleep(400);

						设置.file_rename(设置.游戏根目录 + "\\gw2addon_ReShade64.dll", 设置.游戏根目录 + "\\dxgi.dll");
						Sleep(200);
						if (设置.file_exists(设置.游戏根目录 + "\\dxgi.dll"))
						{
							repugs = true;
						}
					}
					else
					{
						if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, 设置.游戏根目录 + "\\bin64"))
						{
							设置.addlog("解压ReShade64.zip失败! 请重试!");
							错误计数++;
						}
						Sleep(400);

						设置.file_rename(设置.游戏根目录 + "\\bin64\\gw2addon_ReShade64.dll", 设置.游戏根目录 + "\\bin64\\d3d9.dll");
						Sleep(200);
						if (设置.file_exists(设置.游戏根目录 + "\\bin64\\d3d9.dll"))
						{
							repugs = true;
						}
					}
				}
				else
				{
					if (!unzipfs_ss(存放目录 + 设置.全部插件数据[10].Internal_name, 设置.游戏根目录))
					{
						设置.addlog("解压ReShade64.zip失败! 请重试!");
						错误计数++;
					}
					Sleep(200);

					if (设置.全部插件数据[9].used)
					{
						设置.file_rename(设置.游戏根目录 + "\\gw2addon_ReShade64.dll", 设置.游戏根目录 + "\\dxgi.dll");
						Sleep(100);
						if (设置.file_exists(设置.游戏根目录 + "\\dxgi.dll"))
						{
							repugs = true;
						}
					}
					else
					{
						设置.file_rename(设置.游戏根目录 + "\\gw2addon_ReShade64.dll", 设置.游戏根目录 + "\\d3d9.dll");
						Sleep(100);
						if (设置.file_exists(设置.游戏根目录 + "\\d3d9.dll"))
						{
							repugs = true;
						}
					}
				}
			}

			if (repugs)
			{
				设置.addlog("ReShade滤镜插件已安装-dx9模式");
				设置.项目完成数量++;
			}
			else
			{
				设置.addlog("ReShade滤镜插件安装失败! 请重试!");
				错误计数++;
			}
		}
		else if (设置.全部插件数据[12].used)
		{
			bool repugs = false;
			设置.CreateDir(reshard目录);
			if (!unzipfs_ss(存放目录 + 设置.全部插件数据[12].Internal_name, reshard目录))
			{
				设置.addlog("解压GShade.zip失败! 请重试!");
				错误计数++;
			}
			int 计次 = 0;
			while (!设置.file_exists(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name) && 计次 < 20)
			{
				Sleep(100);
				计次++;
			}
			if (计次 < 20)
			{
				if (设置.空中网客户端)
				{

					if (!设置.全部插件数据[0].used)
					{
						if (设置.全部插件数据[9].used)
						{
							设置.file_copy_to(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name, 设置.游戏根目录 + "\\dxgi.dll");
						}
						else
						{
							设置.file_copy_to(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name, 设置.游戏根目录 + "\\bin64\\d3d9.dll");
						}
					}
				}
				else
				{
					if (!设置.全部插件数据[0].used)
					{
						if (设置.全部插件数据[9].used)
						{
							设置.file_copy_to(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name, 设置.游戏根目录 + "\\dxgi.dll");
						}
						else
						{
							设置.file_copy_to(reshard目录 + "\\" + 设置.全部插件数据[12].file_check_name, 设置.游戏根目录 + "\\d3d9.dll");
						}
					}
				}

				设置.file_copy_to(reshard目录 + "\\GShade.ini", 设置.游戏根目录 + "\\GShade.ini");
				repugs = true;
			}
			if (repugs)
			{
				设置.addlog("GShade滤镜插件已安装-dx9模式");
				设置.项目完成数量++;
			}
			else
			{
				设置.addlog("GShade滤镜插件安装失败! 请重试!");
				错误计数++;
			}
		}
		else
		{
			设置.RemoveDir(reshard目录);
		}

		if (设置.全部插件数据[11].used)
		{
			bool swpugs = false;

			if (设置.空中网客户端)
			{
				if (!unzipfs_ss(存放目录 + 设置.全部插件数据[11].Internal_name, bin64))
				{
					设置.addlog("解压ReShade64.zip失败! 请重试!");
					错误计数++;
				}
				Sleep(200);
				设置.file_rename(bin64 + "\\d3d9_chainload.dll", bin64 + "\\d3d9.dll");
				Sleep(200);
				if (设置.file_exists(bin64 + "\\d3d9.dll") && 设置.file_exists(bin64 + "\\dxgi.dll"))
				{
					swpugs = true;
				}
			}
			else
			{
				if (!unzipfs_ss(存放目录 + 设置.全部插件数据[11].Internal_name, 设置.游戏根目录))
				{
					设置.addlog("解压ReShade64.zip失败! 请重试!");
					错误计数++;
				}

				Sleep(200);
				设置.file_rename(设置.游戏根目录 + "\\d3d9_chainload.dll", 设置.游戏根目录 + "\\d3d9.dll");
				Sleep(200);

				if (设置.file_exists(设置.游戏根目录 + "\\d3d9.dll") && 设置.file_exists(设置.游戏根目录 + "\\dxgi.dll"))
				{
					swpugs = true;
				}
			}

			if (swpugs)
			{
				设置.addlog("SweetFX滤镜插件已安装");
				设置.项目完成数量++;
			}
			else
			{
				设置.addlog("SweetFX滤镜插件安装失败! 请重试!");
				错误计数++;
			}
		}
	}

	设置.addlog("删除临时目录");
	设置.RemoveDir(设置.游戏根目录 + "\\addons\\peizi");
	设置.RemoveDir(设置.游戏根目录 + "\\addons\\chajian");

	if (错误计数 > 0)
	{
		string 输出内容 = "安装失败!失败项目数:" + std::to_string(错误计数) + "\r\n您需要尝试重新点击安装!";
		设置.mesgebox("安装失败!", 输出内容);
		设置.addlog(输出内容);
	}
	else
	{
		设置.mesgebox("安装完成!", "请关闭本程序并正常启动游戏\r\n如有问题请前往QQ群寻求帮助\r\nQQ群:\r\n1群:863308792\r\n2群:626045750\r\nQQ群3:1029866604");
		设置.addlog("安装完成!");
	}
	设置.isStarttodo = false;
}

void install::fordolo()
{
	设置.项目总计数量 = 0.0f;
	设置.项目完成数量 = 0.0f;

	if (设置.fileday._Equal("未获取到/获取中"))
	{
		设置.mesgebox("错误!未获知服务器信息", "请获取服务器信息后重试!");
		设置.isStarttodo = false;
		设置.write_log("未获取到服务器信息安装结束");
		return;
	}

	// writelog(to_string(设置.dx11模式));
	for (auto &i : 设置.全部插件数据)
	{

		if (!设置.空中网客户端)
		{
			if (i.id == 97)
			{
				i.used = true;
			}
		}
		//过期代码  d3d9_chainload.dll 调用文件
		// if (全部插件数据[i].id == 11 && 全部插件数据[i].used )
		//{
		//    全部插件数据[12].used = true;
		//}
		if (i.used)
		{
			设置.write_log(i.display_name + "开启");
			设置.项目总计数量++;
		}
	}
	设置.项目总计数量 = 设置.项目总计数量 * 2.0f + 2.0f;

	设置.isStarttodo = true;
	设置.write_log("开始线程安装");
	std::thread cprCall([&]()
						{
							int 项目数量 = 0;
							int 错误数量 = 0;

							try
							{
								for (size_t i = 0; i < 设置.全部插件数据.size(); i++)
								{

									bool needtodow = false;
									//检测文件是否存在
									if (设置.全部插件数据[i].used)
									{
										设置.write_log(设置.全部插件数据[i].display_name + "开始");
										项目数量++;
										if (设置.file_exists(设置.GetExePath() + "\\Installcache\\" + 设置.全部插件数据[i].Internal_name))
										{
											//检测文件md5是否一致
											if (!md5chick2(设置.GetExePath() + "\\Installcache\\" + 设置.全部插件数据[i].Internal_name, 设置.全部插件数据[i].file_md5))
											{

												设置.addlog(设置.全部插件数据[i].display_name + "文件MD5不一致,删除文件");
												设置.write_log(设置.全部插件数据[i].display_name + "文件MD5不一致,删除文件");
												设置.file_delete(设置.GetExePath() + "\\Installcache\\" + 设置.全部插件数据[i].Internal_name);
												needtodow = true;
											}
											else
											{
												设置.全部插件数据[i].is_Download_ok = true;
												设置.全部插件数据[i].is_Download_erro = false;
												设置.项目完成数量++;
											}
										}
										else
										{
											设置.addlog(设置.全部插件数据[i].display_name + "文件未发现开始下载文件");
											设置.write_log(设置.全部插件数据[i].display_name + "文件未发现开始下载文件");
											needtodow = true;
										}
									}

									if (设置.全部插件数据[i].used && !设置.全部插件数据[i].is_inDownload && needtodow)
									{
										设置.全部插件数据[i].is_inDownload = true;
										download(&设置.全部插件数据[i]);
										while (!设置.全部插件数据[i].is_Download_ok)
										{

											Sleep(100);
										}
										if (!设置.全部插件数据[i].is_Download_erro)
										{
											设置.addlog(设置.全部插件数据[i].display_name + "下载完成");
										}
										else
										{
											设置.addlog(设置.全部插件数据[i].display_name + "下载过程中出错!");
										}
										设置.全部插件数据[i].is_inDownload = false;

										Sleep(1000);

										////检测md5
										if (!设置.全部插件数据[i].is_Download_erro && 设置.全部插件数据[i].is_Download_ok && !md5chick2(设置.GetExePath() + "\\Installcache\\" + 设置.全部插件数据[i].Internal_name, 设置.全部插件数据[i].file_md5))
										{
											设置.全部插件数据[i].is_Download_erro = true;
											设置.addlog(设置.全部插件数据[i].display_name + "的文件MD5校验不一致请重新下载!");
											设置.write_log(设置.全部插件数据[i].display_name + "的文件MD5校验不一致请重新下载!");
										}
										if (设置.全部插件数据[i].is_Download_erro)
											错误数量++;
										设置.项目完成数量++;
									}
								}
								设置.addlog("项目数量:" + to_string(项目数量) + "错误数量:" + to_string(错误数量));
								if (错误数量 > 0)
								{
									设置.addlog("下载过程中有错误项目!请重试!");
									设置.mesgebox("请重试!", "下载过程中有错误的项目,您需要重新下载!");
								}
								else
								{
									设置.addlog("全部下载完成!开始安装进程!");
									//开始安装
									设置.write_log("全部下载判断完成!开始安装进程!!");
									安装逻辑();
								}
							}
							catch (const std::exception &e)
							{
								设置.addlog("安装进程中出错!");
								设置.addlog(e.what());
								设置.isStarttodo = false;
								设置.mesgebox("安装进程中出错!", e.what());
								设置.write_log("安装进程中出错!!");
								设置.write_log(e.what());
							}
						});
	cprCall.detach();
}
