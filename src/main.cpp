
#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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

	window = SDL_CreateWindow("Base", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL |SDL_WINDOW_ALLOW_HIGHDPI );
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

		            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

		ImGui::ShowDemoWindow();
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	shutdown();

	return 0;
}

//和我		

