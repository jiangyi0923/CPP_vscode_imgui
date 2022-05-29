#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "resource.h"
#include <shellapi.h>

#include "global.h"
#include "getinfos.h"
#include "install.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "tlhelp32.h"
#pragma comment(lib, "Msi.lib")

using namespace std;

#ifdef _WIN32
extern "C"
{
	__declspec(dllexport) bool NvOptimusEnablement = true;
	__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
}
#endif

SDL_Window *window = nullptr;
SDL_GLContext gl_context = nullptr;
void *fontPtr;
size_t fontSize;

int my_image_width = 0;
int my_image_height = 0;
GLuint my_image_texture = 0;

int zfb_width = 0;
int zfb_height = 0;
GLuint zfb_texture = 0;

//加载字体
bool LoadFontResource(UINT resId, void *&dataPtr, size_t &dataSize)
{
	auto res = FindResource(nullptr, MAKEINTRESOURCE(resId), RT_RCDATA);
	if (res)
	{
		auto handle = LoadResource(nullptr, res);
		if (handle)
		{
			dataSize = SizeofResource(nullptr, res);
			dataPtr = LockResource(handle);

			return true;
		}
	}
	return false;
}

bool LoadTextureFromFile(UINT resId, GLuint *out_texture, int *out_width, int *out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	void *dataPtr;
	size_t dataSize;
	auto res = FindResource(nullptr, MAKEINTRESOURCE(resId), RT_RCDATA);
	if (res)
	{
		auto handle = LoadResource(nullptr, res);
		if (handle)
		{
			dataSize = SizeofResource(nullptr, res);
			dataPtr = LockResource(handle);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	unsigned char *image_data = stbi_load_from_memory((const stbi_uc *)dataPtr, (int)dataSize, &image_width, &image_height, NULL, 4);

	// unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

int foundProcess360()
{
	string program_name = "360Safe.exe";
	string program_name_1 = "360sd.exe";
	string program_name_2 = "360tray.exe";
	bool ret = false;
	HANDLE info_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //拍摄系统中所有进程的快照
	if (info_handle == INVALID_HANDLE_VALUE)
	{
		// printf("CreateToolhelp32Snapshot fail!!\n\n");
		return false;
	}

	tagPROCESSENTRY32 program_info;
	program_info.dwSize = sizeof(tagPROCESSENTRY32);							//设置结构体大小
	int bResult = Process32First(info_handle, (LPPROCESSENTRY32)&program_info); //获取所有进程中第一个进程的信息
	if (!bResult)
	{
		// printf("Process32FirstW fail!!\n\n");
		return false;
	}
	while (bResult)
	{
		// char* pro_name = Wchar2char(program_info.szExeFile);
		//输出内容 += string(program_info.szExeFile) + "\r\n";//GW2WGLauncher.exe
		if (program_name._Equal(string(program_info.szExeFile)) ||
			program_name_1._Equal(string(program_info.szExeFile)) ||
			program_name_2._Equal(string(program_info.szExeFile)))
		{
			ret = true;
			break;
		}
		//获得下一个进程的进程信息
		bResult = Process32Next(info_handle, (LPPROCESSENTRY32)&program_info);
	}
	CloseHandle(info_handle); //关闭句柄
	return ret;
}

//检测激战2是否在运行
int foundProcessByName()
{
	string program_name = "Gw2-64.exe";
	string program_name1 = "GW2WGLauncher.exe";
	string program_name2 = "GW2Lanucher.exe";
	bool ret = false;
	HANDLE info_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //拍摄系统中所有进程的快照
	if (info_handle == INVALID_HANDLE_VALUE)
	{
		// printf("CreateToolhelp32Snapshot fail!!\n\n");
		return false;
	}

	tagPROCESSENTRY32 program_info;
	program_info.dwSize = sizeof(tagPROCESSENTRY32);							//设置结构体大小
	int bResult = Process32First(info_handle, (LPPROCESSENTRY32)&program_info); //获取所有进程中第一个进程的信息
	if (!bResult)
	{
		// printf("Process32FirstW fail!!\n\n");
		return false;
	}

	while (bResult)
	{
		// char* pro_name = Wchar2char(program_info.szExeFile);
		//输出内容 += string(program_info.szExeFile) + "\r\n";//GW2WGLauncher.exe
		if (program_name._Equal(string(program_info.szExeFile)) || program_name1._Equal(string(program_info.szExeFile)) || program_name2._Equal(string(program_info.szExeFile)))
		{
			ret = true;
			break;
		}
		//获得下一个进程的进程信息
		bResult = Process32Next(info_handle, (LPPROCESSENTRY32)&program_info);
	}
	CloseHandle(info_handle); //关闭句柄
	return ret;
}

void setup()
{

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	设置.load_set();
	int x, y;
	if (设置.字体大小 > 15.0f)
	{
		x = int(400 * (设置.字体大小 * 0.005f + 1));
		y = int(620 * (设置.字体大小 * 0.005f + 1));
	}
	else
	{
		x = 400;
		y = 620;
	}

	window = SDL_CreateWindow("激战2插件在线安装工具 V2.1.4b", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// SDL_SetWindowSize(window,x,y);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	gladLoadGL();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	auto &imio = ImGui::GetIO();

	switch (设置.主题样式)
	{
	case 0:
		ImGui::StyleColorsDark();
		break;
	case 1:
		ImGui::StyleColorsLight();
		break;
	case 2:
		ImGui::StyleColorsClassic();
		break;
	default:
		break;
	}

	imio.IniFilename = nullptr;
	auto fontCfg = ImFontConfig();
	fontCfg.FontDataOwnedByAtlas = false;

	if (LoadFontResource(ID_FONT, fontPtr, fontSize))
	{
		io.Fonts->AddFontFromMemoryTTF(fontPtr, int(fontSize), 设置.字体大小, &fontCfg, io.Fonts->GetGlyphRangesChineseFull());
	}

	if (LoadTextureFromFile(ID_PD, &my_image_texture, &my_image_width, &my_image_height))
	{
		/* code */
	}
	if (LoadTextureFromFile(ID_ZFB, &zfb_texture, &zfb_width, &zfb_height))
	{
		/* code */
	}

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init();
}

void shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	window = nullptr;
	gl_context = nullptr;
	SDL_Quit();
	// ImGui::Image();
}

static void HelpMarker(const char *desc)
{
	ImGui::TextDisabled("(帮助)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}





int main(int, char **)
{
	if (foundProcess360())
	{
		string pszMsg = "检测到您正在运行垃圾软件360\r\n请关闭或者卸载这个垃圾软件\r\n本程序将会关闭!";
		string pszTitle = "您正在运行垃圾软件!";

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
		return 1;
	}

	if (foundProcessByName())
	{
		string pszMsg = "检测到您正在运行激战2程序\r\n请关闭激战2后安装插件\r\n本程序将会关闭!";
		string pszTitle = "激战2正在运行!";

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
		return 1;
	}
	if (设置.file_exists(设置.GetExePath() + "\\Gw2-64.exe"))
	{
		设置.游戏根目录 = 设置.GetExePath();
		设置.addlog("当前目录:" + 设置.游戏根目录);
		if (设置.mincjianc())
		{
			//有中文
			string pszMsg = "检测到您当前的激战2目录文件夹名称中有中文\r\n为避免出现不良现象请重命名文件夹\r\n列如\"C:\\Program Files\\Guild Wars 2 \"这样的非中文目录 \r\n本程序将会关闭!";
			string pszTitle = "激战2目录文件夹名称中有中文!!";

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
			return 1;
		}
		else
		{

			设置.CreateDir("Installcache");
			设置.CreateDir("addons");
			设置.CreateDir("addons\\arcdps");
			设置.CreateDir("bin64");
			设置.open_log();
			设置.write_log("前置判断完成");

			if (设置.file_exists(设置.游戏根目录 + "\\GW2WGLauncher.exe"))
			{
				设置.空中网客户端 = false;
				设置.addlog("客户端:WG版");
				设置.write_log("客户端:WG版");
			}
			else if (设置.file_exists(设置.游戏根目录 + "\\GW2Lanucher.exe"))
			{
				设置.空中网客户端 = false;
				设置.addlog("客户端:顺网版或其他");
				设置.write_log("客户端:顺网版或其他");
			}
			else
			{
				设置.空中网客户端 = true;
				设置.addlog("客户端:空中网或外服");
				设置.write_log("客户端:空中网或外服");
			}

			设置.Versionchick();
			if (设置.vc版本不合格)
			{
				设置.mesgebox("vc++版本不合格", "检测到您安装的vc++版本过低或未安装\r\n请前往QQ群或百度下载安装VC++2015-2019/VC++2015-2022 V14.27以上64位版本!");
				设置.write_log("vc++版本不合格");
			}
		}
	}
	else
	{
		string pszMsg = "检测到您当前的目录中未发现Gw2-64.exe程序\r\n请将本程序放置在游戏根目录下\r\n本程序将会关闭!";
		string pszTitle = "未发现Gw2-64.exe!";

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
		return 1;
	}

	setup();
	getinfos gqw;
	install 安装;

	bool running = true;
	while (running)
	{

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{

			ImGui_ImplSDL2_ProcessEvent(&e);

			switch (e.type)
			{
			case SDL_QUIT:
			{
				running = false;
			}
			break;
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ImGui::ShowUserGuide();

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		ImGui::SetNextWindowPos({0.f, 0.f});

		float xw, yw;
		if (设置.字体大小 > 15.0f)
		{
			xw = (400 * (设置.字体大小 * 0.005f + 1));
			yw = (620 * (设置.字体大小 * 0.005f + 1));
		}
		else
		{
			xw = 400.f;
			yw = 620.f;
		}

		ImGui::SetNextWindowSize({xw, yw});
		ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration); // Create a window called "Hello, world!" and append into it.

		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("安装和更新"))
			{
				ImGui::Text("最新文件日期: %s", 设置.fileday.c_str());
				ImGui::Text("游戏DX版本:");
				if (设置.isStarttodo)
					ImGui::BeginDisabled(true);
				//ImGui::RadioButton("DX9模式", &设置.dx11模式, 0);
				//ImGui::SameLine();
				ImGui::RadioButton("DX11模式", &设置.dx11模式, 1);
				ImGui::SameLine();
				ImGui::Text("因DPS插件不再支持dx9,现已全面禁用DX9模式");

				ImGui::Text("ARCDPS系列插件:");
				ImVec2 child_rect_min = ImGui::GetItemRectSize();

				for (size_t i = 0; i < 9; i++)
				{
					//如果dps插件禁用系列内其他插件同样禁用
					if (i != 0 && !设置.全部插件数据[0].used)
					{
						设置.全部插件数据[i].used = false;
					}

					if (ImGui::Checkbox(设置.全部插件数据[i].display_name.c_str(), &设置.全部插件数据[i].used))
					{
						if (设置.全部插件数据[i].used && i == 1)
						{
							设置.mesgebox("兼容提醒!", "此款插件兼容性堪忧请谨慎安装!\r\n已知问题:pvp和战场概率报错,大部分未知情况下报错");
						}
					}
					ImGui::SameLine();
					HelpMarker(设置.全部插件数据[i].helpstring.c_str());
					ImGui::SameLine(child_rect_min.x * 1.8f);

					if (设置.全部插件数据[i].is_Download_erro)
					{
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.89f, 0.15f, 0.15f, 1.0f});
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.15f, 0.89f, 0.15f, 1.0f});
					}
					if (设置.全部插件数据[i].is_Download_ok && !设置.全部插件数据[i].is_Download_erro)
					{
						设置.全部插件数据[i].dowpr = 1.0f;
					}
					ImGui::ProgressBar(设置.全部插件数据[i].dowpr, ImVec2(child_rect_min.x * 1.8f, 0.f));
					ImGui::PopStyleColor(1);
				}
				ImGui::Text("其他插件:");

				if (设置.dx11模式 == 0)
				{
					if (ImGui::Checkbox(设置.全部插件数据[9].display_name.c_str(), &设置.全部插件数据[9].used))
					{
						if (设置.全部插件数据[9].used)
						{
							设置.mesgebox("兼容提醒!", "近期此款插件兼容性堪忧请谨慎安装!\r\n已知问题:大部分未知情况下报错或闪退");
						}
					}
					ImGui::SameLine();
					HelpMarker(设置.全部插件数据[9].helpstring.c_str());
					ImGui::SameLine(child_rect_min.x * 1.8f);
					if (设置.全部插件数据[9].is_Download_erro)
					{
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.89f, 0.15f, 0.15f, 1.0f});
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.15f, 0.89f, 0.15f, 1.0f});
					}
					if (设置.全部插件数据[9].is_Download_ok && !设置.全部插件数据[9].is_Download_erro)
					{
						设置.全部插件数据[9].dowpr = 1.0f;
					}
					ImGui::ProgressBar(设置.全部插件数据[9].dowpr, ImVec2(child_rect_min.x * 1.8f, 0.f));
					ImGui::PopStyleColor(1);
					if (设置.全部插件数据[9].used)
					{
						ImGui::RadioButton("4+16G", &设置.d912pxy_配置, 0);
						ImGui::SameLine();
						ImGui::RadioButton("8+16G", &设置.d912pxy_配置, 1);
						ImGui::SameLine();
						ImGui::RadioButton("低配置", &设置.d912pxy_配置, 2);
					}
				}
				else
				{
					设置.全部插件数据[9].used = false;
				}

				ImGui::Checkbox(设置.全部插件数据[10].display_name.c_str(), &设置.全部插件数据[10].used);
				ImGui::SameLine();
				HelpMarker(设置.全部插件数据[10].helpstring.c_str());
				ImGui::SameLine(child_rect_min.x * 1.8f);
				if (设置.全部插件数据[10].is_Download_erro)
				{
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.89f, 0.15f, 0.15f, 1.0f});
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.15f, 0.89f, 0.15f, 1.0f});
				}
				if (设置.全部插件数据[10].is_Download_ok && !设置.全部插件数据[10].is_Download_erro)
				{
					设置.全部插件数据[10].dowpr = 1.0f;
				}
				ImGui::ProgressBar(设置.全部插件数据[10].dowpr, ImVec2(child_rect_min.x * 1.8f, 0.f));
				ImGui::PopStyleColor(1);

				ImGui::Checkbox(设置.全部插件数据[12].display_name.c_str(), &设置.全部插件数据[12].used);
				ImGui::SameLine();
				HelpMarker(设置.全部插件数据[12].helpstring.c_str());
				ImGui::SameLine(child_rect_min.x * 1.8f);
				if (设置.全部插件数据[12].is_Download_erro)
				{
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.89f, 0.15f, 0.15f, 1.0f});
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.15f, 0.89f, 0.15f, 1.0f});
				}
				if (设置.全部插件数据[12].is_Download_ok && !设置.全部插件数据[12].is_Download_erro)
				{
					设置.全部插件数据[12].dowpr = 1.0f;
				}
				ImGui::ProgressBar(设置.全部插件数据[12].dowpr, ImVec2(child_rect_min.x * 1.8f, 0.f));
				ImGui::PopStyleColor(1);

				if (设置.dx11模式 == 0)
				{
					if (!设置.全部插件数据[0].used)
					{
						ImGui::Checkbox(设置.全部插件数据[11].display_name.c_str(), &设置.全部插件数据[11].used);
						ImGui::SameLine();
						HelpMarker(设置.全部插件数据[11].helpstring.c_str());
						ImGui::SameLine(child_rect_min.x * 1.8f);
						if (设置.全部插件数据[11].is_Download_erro)
						{
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.89f, 0.15f, 0.15f, 1.0f});
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.15f, 0.89f, 0.15f, 1.0f});
						}
						if (设置.全部插件数据[11].is_Download_ok && !设置.全部插件数据[11].is_Download_erro)
						{
							设置.全部插件数据[11].dowpr = 1.0f;
						}
						ImGui::ProgressBar(设置.全部插件数据[11].dowpr, ImVec2(child_rect_min.x * 1.8f, 0.f));
						ImGui::PopStyleColor(1);
					}
					else
					{
						设置.全部插件数据[11].used = false;
					}
				}
				else
				{
					设置.全部插件数据[11].used = false;
				}

				if (设置.全部插件数据[9].used || 设置.全部插件数据[10].used || 设置.全部插件数据[12].used)
					设置.全部插件数据[11].used = false;
				if (设置.全部插件数据[11].used)
					设置.全部插件数据[9].used = 设置.全部插件数据[10].used = 设置.全部插件数据[12].used = false;
				if (设置.全部插件数据[10].used)
					设置.全部插件数据[12].used = false;
				if (设置.全部插件数据[12].used)
					设置.全部插件数据[10].used = false;

				for (size_t i = 0; i < 设置.全部插件数据.size(); i++)
				{
					if ((设置.全部插件数据[i].id == 96 ||
						 设置.全部插件数据[i].id == 97 ||
						 设置.全部插件数据[i].id == 98 ||
						 设置.全部插件数据[i].id == 99) &&
						设置.全部插件数据[i].is_inDownload && 设置.全部插件数据[i].used)
					{
						ImGui::Text(设置.全部插件数据[i].display_name.c_str());
						ImGui::SameLine(child_rect_min.x * 1.8f);
						if (设置.全部插件数据[i].is_Download_erro)
						{
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.89f, 0.15f, 0.15f, 1.0f});
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.15f, 0.89f, 0.15f, 1.0f});
						}
						if (设置.全部插件数据[i].is_Download_ok && !设置.全部插件数据[i].is_Download_erro)
						{
							设置.全部插件数据[i].dowpr = 1.0f;
						}
						ImGui::ProgressBar(设置.全部插件数据[i].dowpr, ImVec2(child_rect_min.x * 1.8f, 0.f));
						ImGui::PopStyleColor(1);
					}
				}
				if (设置.isStarttodo)
					ImGui::EndDisabled();

				if (设置.首次使用)
				{
					static vector<string> 列表文{
						"模式",
						"窗口全屏",
						"我已设置",
						"游戏为",
					};
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.0f, 0.2f, 1.0f, 1.0f});
					ImGui::Text("左右拖动下列按钮使其正确排序\r\n排序正确既可点击安装插件按钮");
					ImGui::PopStyleColor(1);
					for (int n = 0; n < 4; n++)
					{
						ImGui::PushID(n);
						if ((n % 4) != 0)
							ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.5f, 0.5f, 1.0f});
						ImGui::Button(列表文[n].c_str());
						ImGui::PopStyleColor(1);
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
						{
							ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));
							ImGui::EndDragDropSource();
						}
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
							{
								IM_ASSERT(payload->DataSize == sizeof(int));
								int payload_n = *(const int *)payload->Data;
								string tmp = 列表文[n].c_str();
								列表文[n] = 列表文[payload_n];
								列表文[payload_n] = tmp;
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PopID();
					}
					if (列表文[0]._Equal("我已设置") && 列表文[1]._Equal("游戏为") && 列表文[2]._Equal("窗口全屏") && 列表文[3]._Equal("模式"))
					{
						设置.输入正确 = true;
						设置.首次使用 = false;
					}
				}
				else
				{
					if (设置.限制安装)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.0f, 0.0f, 1.0f, 1.0f});
						ImGui::Text("请在 %d月%d日 游戏更新后安装", 设置.m + 1, 设置.d);
						ImGui::Text("非美服客户端 提前安装后果自负!");
						ImGui::PopStyleColor(1);
						if (ImGui::Button("确定并关闭工具"))
						{
							return 0;
						}
						ImGui::SameLine();
						if (ImGui::Button("我要提前安装"))
						{
							设置.限制安装 = false;
						}
					}
					else
					{
						if (!设置.isStarttodo)
						{
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.0f, 0.2f, 1.0f, 1.0f});
							ImGui::Text("请确认已将游戏设置为窗口全屏模式!");
							ImGui::Text("请确认已将游戏设置为窗口全屏模式!");
							ImGui::Text("请确认已将游戏设置为窗口全屏模式!");

							ImGui::PopStyleColor(1);
							if (ImGui::Button("正常安装/更新"))
							{
								if (!设置.isStarttodo)
								{
									设置.首次使用 = false;
									设置.isStarttodo = true;
									设置.save_set();
									设置.addlog("开始下载进程");
									安装.是卸载 = false;
									安装.是疑难安装 = false;
									安装.fordolo();
								}
							}
							ImGui::SameLine();
							if (设置.dx11模式 == 1)
							{
								if (ImGui::Button("疑难安装/更新"))
								{
									if (!设置.isStarttodo)
									{
										设置.首次使用 = false;
										设置.isStarttodo = true;
										设置.save_set();
										设置.addlog("开始下载进程");

										安装.是卸载 = false;
										安装.是疑难安装 = true;
										安装.fordolo();
									}
								}
								if (ImGui::IsItemHovered())
								{
									ImGui::BeginTooltip();
									ImGui::Text("此方法只针对正常安装后DX11插件不生效的用户!!\r\n不保证此方法亦能生效");
									ImGui::EndTooltip();
								}
								ImGui::SameLine();
							}
							if (ImGui::Button("删除addons目录"))
							{
								设置.addlog("开始删除addons目录");
								if (GetFileAttributesA((设置.游戏根目录 + "\\addons").c_str()))
								{
									设置.RemoveDir(设置.游戏根目录 + "\\addons");
								}
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("这将会删除游戏目录下addons目录\r\n此目录下存放有插件配置文件\r\n请谨慎操作!删除后您需要重新设置插件的选项!");
								ImGui::EndTooltip();
							}
							if (设置.dx11模式 == 0)
							{
								ImGui::SameLine();
							}

							if (ImGui::Button("卸载插件"))
							{
								设置.addlog("开始卸载插件");

								安装.是卸载 = false;
								安装.是疑难安装 = false;
								安装.卸载逻辑();
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("这将会删除游戏目录下addons目录\r\n此目录下存放有插件配置文件\r\n请谨慎操作!删除后您再安装需要重新设置插件的选项!");
								ImGui::EndTooltip();
							}
						}
						else
						{
							ImGui::Text("正在行动中");
							ImGui::ProgressBar(设置.项目进度(), ImVec2(-1.0f, 0.0f));
						}
					}

					if (设置.apierro > 0)
					{
						if (ImGui::Button("解决方法1:尝试重新从服务器获取插件数据信息"))
						{
							//设置.getdatainfo();
							gqw.getto(0);
						}
						if (ImGui::Button("解决方法2.步骤1:获取data.txt文件"))
						{
							设置.mesgebox("获取本地data.txt文件说明", "请使用快捷键 CTRL + A 复制打开的浏览器网页中全部内容,在Installcache目录下新建名为data的文本文档(即data.txt),打开它粘贴你复制的内容并保存(保证第一行无空格符或回车符并且文件为UTF-8模式)后,点击-解析本地data.txt文件-按钮\r\n点击确定打开网页");
							ShellExecuteA(0, 0, "https://gitee.com/api/v5/repos/jiangyi0923/gw2chajianfile/releases/latest?access_token=ea9e8776dbc01bd019ca905d0418c6bf", 0, 0, SW_SHOW);
						}
						if (ImGui::Button("解决方法2.步骤2:解析本地data.txt文件"))
						{
							gqw.getto(1);
						}
					}
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("安装日志"))
			{

				ImGui::BeginChild("##optx", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
				ImGui::TextUnformatted(设置.output.c_str());
				if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() * 0.95f)
				{
					ImGui::SetScrollHereY(1.0f);
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("帮助"))
			{

				ImGui::BeginChild("##optx22w", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
				ImGui::BeginChildFrame(ImGui::GetID("tutorial"), ImVec2(0, 380));
				ImGui::TextWrapped("可前往QQ群或百度下载安装运行库\r\n"
								   "如需sct图标包请前往QQ群下载\r\nQQ群1:863308792\r\nQQ群2:626045750\r\nQQ群3:1029866604\r\n"
								   "请注意!请注意!请注意!\r\n"
								   "1.你可能需要关闭\r\nGeforce Experinece 游戏内覆盖功能\r\n"
								   "2.本工具已改用addonLoader加载插件\r\n"
								   "3.安装dps插件情况下所有插件位置都在addon目录下\r\n\r\n\r\n"
								   "-----字体变成方块-----\r\n"
								   "情况一，字体大小调太大了，解决方法:\r\n"
								   "关闭游戏删除addons/arcdps/arcdps.ini后重启游戏即可\r\n"
								   "根据屏幕分辨率推荐大小:\r\n"
								   "1k屏幕 推荐 13-15\r\n"
								   "2k屏幕 推荐 18-20\r\n"
								   "4k屏幕 推荐 22-24\r\n"
								   "否则会很大很大!设置完成后请重启游戏\r\n"
								   "情况二，dx12插件还没加载完全等会就好了\r\n"
								   "-----兼容问题-----\r\n"
								   "已知以下三款硬件级应用存在兼容问题\r\n"
								   "1.MSI Afterburner(微星设备)\r\n"
								   "2.RivaTuner Statistics Server / RTSS(NVIDIA显卡修改工具)\r\n"
								   "3.雷云Razer synapse(雷蛇设备)\r\n"
								   "请在游戏运行前关闭或者卸载该应用!\r\n"
								   "近期D912PXY插件和SCT流动输出插件兼容性堪忧\r\n"
								   "-----汉化丢失-----\r\n"
								   "游戏目录 / addons / arcdps / arcdps_font.ttf  字体文件\r\n"
								   "游戏目录 / addons / arcdps / arcdps_long.ini  汉化文件\r\n"
								   "如果插件界面为英文请确认是否缺失以上文件\r\n"
								   "如果插件界面文字为？号请确认游戏目录名是否包含中文\r\n"
								   "-----更新常识-----\r\n"
								   "首先明确：DPS插件跟随美服版本更新而更新\r\n"
								   "美服一般北京时间每周三凌晨零点更新\r\n"
								   "国服延迟到每周五早上8点更新\r\n"
								   "有时候版本更新时会更新技能参数\r\n"
								   "或者添加修改某些参数地址\r\n"
								   "如果在国服没更新的情况下更新了插件就会造成\r\n"
								   "闪退, 白屏, 报错, 卡屏等等一些不可预知的后果！\r\n"
								   "所以周三周四不要更新插件\r\n"

				);
				ImGui::EndChildFrame();

				if (ImGui::SmallButton("前往ARCDPS作者网站"))
				{
					ShellExecuteA(0, 0, "https://www.deltaconnected.com/arcdps/", 0, 0, SW_SHOW);
				}

				ImGui::SameLine();

				if (ImGui::SmallButton("前往配装板作者网站"))
				{
					ShellExecuteA(0, 0, "https://buildpad.gw2archive.eu/", 0, 0, SW_SHOW);
				}

				if (ImGui::SmallButton("前往治疗统计作者网站"))
				{
					ShellExecuteA(0, 0, "https://github.com/Krappa322/arcdps_healing_stats/releases", 0, 0, SW_SHOW);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("前往团队机制作者网站"))
				{
					ShellExecuteA(0, 0, "https://github.com/knoxfighter/GW2-ArcDPS-Mechanics-Log/releases", 0, 0, SW_SHOW);
				}

				if (ImGui::SmallButton("前往团队增益作者网站"))
				{
					ShellExecuteA(0, 0, "https://github.com/knoxfighter/GW2-ArcDPS-Boon-Table/releases", 0, 0, SW_SHOW);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("前往SCT流动输出作者网站"))
				{
					ShellExecuteA(0, 0, "https://github.com/Artenuvielle/GW2-SCT/releases", 0, 0, SW_SHOW);
				}

				if (ImGui::SmallButton("前往d912pxy作者网站"))
				{
					ShellExecuteA(0, 0, "https://github.com/megai2/d912pxy/releases", 0, 0, SW_SHOW);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("前往Reshade官方网站"))
				{
					ShellExecuteA(0, 0, "https://reshade.me", 0, 0, SW_SHOW);
				}
				if (ImGui::SmallButton("前往本工具开源项目库"))
				{
					ShellExecuteA(0, 0, "https://github.com/jiangyi0923/CPP_vscode_imgui", 0, 0, SW_SHOW);
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("主题"))
			{

				if (ImGui::Button("黑色主题"))
				{
					ImGui::StyleColorsDark();
					设置.主题样式 = 0;
					设置.save_set();
				}
				ImGui::SameLine();
				if (ImGui::Button("白色主题"))
				{
					ImGui::StyleColorsLight();
					设置.主题样式 = 1;
					设置.save_set();
				}
				ImGui::SameLine();
				if (ImGui::Button("经典主题"))
				{
					ImGui::StyleColorsClassic();
					设置.主题样式 = 2;
					设置.save_set();
				}
				ImGui::SetNextItemWidth(120.f);
				if (ImGui::InputFloat("本工具字体大小", &设置.字体大小, 1.0f, 2.0f, "%.0f"))
				{
					
					if (设置.字体大小 > 24.0f || 设置.字体大小 < 15.0f)
					{
						int x, y;
						if (设置.字体大小 > 24.0f)
							设置.字体大小 = 24.0f;
						if (设置.字体大小 < 15.0f)
							设置.字体大小 = 15.0f;
						if (设置.字体大小 > 15.0f)
						{
							x = int(400 * (设置.字体大小 * 0.005f + 1));
							y = int(620 * (设置.字体大小 * 0.005f + 1));
						}
						else
						{
							x = 400;
							y = 620;
						}
						SDL_SetWindowSize(window, x, y);
						设置.save_set();
					}
					
				}
				ImGui::Text("更改本工具字体大小需要重启生效!");

				ImGui::SetNextItemWidth(120.f);
				if (ImGui::InputInt("ARCDPS插件字体大小", &设置.DPS字体大小))
				{
					if (设置.DPS字体大小 > 24 || 设置.DPS字体大小 < 13)
					{
						if (设置.DPS字体大小 > 24)
							设置.DPS字体大小 = 24;
						if (设置.DPS字体大小 < 13)
							设置.DPS字体大小 = 13;
					}
					设置.save_dps();
				}
				ImGui::Text("更改ARCDPS插件字体大小需关闭游戏时设置!");

				ImGui::InvisibleButton("23rwfre", {1, 1});
				ImGui::InvisibleButton("23rwfre", {1, 1});
				ImGui::InvisibleButton("23rwfre", {1, 1});
				ImGui::InvisibleButton("23rwfre", {1, 1});
				ImGui::SameLine((ImGui::GetContentRegionAvail().x - ImGui::GetContentRegionAvail().x * 0.7f) * 0.5f);
				ImGui::BeginChild("##optx2222zfb", ImVec2(0, 0), false, ImGuiWindowFlags_NoNav);
				ImGui::Text("如果喜欢本应用请不吝打赏(支付宝)");
				ImGui::Image((void *)(intptr_t)zfb_texture, ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, ImGui::GetContentRegionAvail().x * 0.7f));
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("QQ频道"))
			{
				if (ImGui::Button("加入QQ频道", ImVec2(ImGui::GetWindowContentRegionMax().x, 50.f)))
				{
					ShellExecuteA(0, 0, "https://qun.qq.com/qqweb/qunpro/share?_wv=3&_wwv=128&appChannel=share&inviteCode=1W4qIWf&appChannel=share&businessType=9&from=246610&biz=ka", 0, 0, SW_SHOW);
				}

				ImGui::InvisibleButton("退了", {1, 1});
				ImGui::SameLine((ImGui::GetContentRegionAvail().x - (float)my_image_width) * 0.5f);
				ImGui::BeginChild("##optx2222w", ImVec2(0, 0), false, ImGuiWindowFlags_NoNav);
				ImGui::Text("请使用手机QQ扫一扫加入频道");
				ImGui::Image((void *)(intptr_t)my_image_texture, ImVec2((float)my_image_width, (float)my_image_height));
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}

		if (gqw.thisonce)
		{
			/* code */
			gqw.thisonce = false;
			gqw.getto(0);
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	shutdown();

	return 0;
}
