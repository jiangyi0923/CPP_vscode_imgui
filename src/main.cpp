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
//#include <cpr/cpr.h>

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

void setup()
{

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("激战2插件在线安装工具", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	// SDL_iconv

	// SDL_LoadFile_RW();

	// SDL_Surface cursor = SDL_CreateRGBSurfaceFrom(LoadIcon(nullptr, MAKEINTRESOURCE(IDI_ICON1)),16,16,16,16*2,0x0f00,0x00f0,0x000f,0xf000);
	// SDL_SetWindowIcon(window,&cursor);

	gladLoadGL();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	//	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simhei.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	// IM_ASSERT(font != NULL);
	auto &imio = ImGui::GetIO();
	imio.IniFilename = nullptr;
	auto fontCfg = ImFontConfig();
	fontCfg.FontDataOwnedByAtlas = false;

	if (LoadFontResource(ID_FONT, fontPtr, fontSize))
	{
		io.Fonts->AddFontFromMemoryTTF(fontPtr, int(fontSize), 18.0f, &fontCfg, io.Fonts->GetGlyphRangesChineseFull());
		// IM_ASSERT(font_ != nullptr);
	}

	// cpr::Response response = cpr::Get(cpr::Url{ link }, cpr::Header{ {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"} });

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init();
}

string fewtext;

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
}

//加载字体

void download(const char *Url, const char *save_as) /*将Url指向的地址的文件下载到save_as指向的本地文件*/
{
	BYTE Temp[1024];
	ULONG Number = 1;

	FILE *stream;
	HINTERNET hSession = InternetOpenA("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET handle2 = InternetOpenUrlA(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (handle2 != NULL)
		{

			if (fopen_s(&stream, save_as, "wb") != EINVAL)
			{
				while (Number > 0)
				{
					InternetReadFile(handle2, Temp, 1024 - 1, &Number);
					// printf_s(to_string(Number).c_str());
					fewtext += to_string(Number + 1) + "\r\n";
					fwrite(Temp, sizeof(char), Number, stream);
				}
				fclose(stream);
			}

			InternetCloseHandle(handle2);
			handle2 = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

int main(int, char **)
{

	setup();

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
		 ImGui::SetNextWindowSize({ 340.f,550.f });
		ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration); // Create a window called "Hello, world!" and append into it.
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("安装和更新"))
			{

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
					ShellExecute(0, 0, "https://www.deltaconnected.com/arcdps/", 0, 0, SW_SHOW);
				}

				ImGui::SameLine();

				if (ImGui::SmallButton("前往配装板作者网站"))
				{
					ShellExecute(0, 0, "https://buildpad.gw2archive.eu/", 0, 0, SW_SHOW);
				}

				if (ImGui::SmallButton("前往治疗统计作者网站"))
				{
					ShellExecute(0, 0, "https://github.com/Krappa322/arcdps_healing_stats/releases", 0, 0, SW_SHOW);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("前往团队机制作者网站"))
				{
					ShellExecute(0, 0, "https://github.com/knoxfighter/GW2-ArcDPS-Mechanics-Log/releases", 0, 0, SW_SHOW);
				}

				if (ImGui::SmallButton("前往团队增益作者网站"))
				{
					ShellExecute(0, 0, "https://github.com/knoxfighter/GW2-ArcDPS-Boon-Table/releases", 0, 0, SW_SHOW);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("前往SCT流动输出作者网站"))
				{
					ShellExecute(0, 0, "https://github.com/Artenuvielle/GW2-SCT/releases", 0, 0, SW_SHOW);
				}

				if (ImGui::SmallButton("前往d912pxy作者网站"))
				{
					ShellExecute(0, 0, "https://github.com/megai2/d912pxy/releases", 0, 0, SW_SHOW);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("前往Reshade官方网站"))
				{
					ShellExecute(0, 0, "https://reshade.me", 0, 0, SW_SHOW);
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	shutdown();

	return 0;
}
