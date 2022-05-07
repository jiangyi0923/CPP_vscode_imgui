#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "resource.h"

#include "global.h"
#include <thread>

#pragma comment(lib, "wininet.lib")
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

void setup()
{

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("Base", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	gladLoadGL();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simhei.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	// IM_ASSERT(font != NULL);

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
					//printf_s(to_string(Number).c_str());
					fewtext += to_string(Number+1) + "\r\n";
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




		ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

		for (size_t i = 0; i < 设置.全部插件数据.size(); i++)
		{
			if (ImGui::Checkbox(设置.全部插件数据[i].display_name.c_str(),&设置.全部插件数据[i].used))
			{
				设置.addlog	(设置.全部插件数据[i].display_name + (设置.全部插件数据[i].used? "启用":"禁用"));
			}
		}
		

		if (ImGui::Button("下载"))
		{
			thread t2(download, "https://www.wemod.com/download/direct", "375480.exe");
			t2.detach();
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::TextUnformatted(fewtext.c_str());
		ImGui::TextUnformatted(设置.output.c_str());
		ImGui::End();
				//ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	shutdown();

	return 0;
}
