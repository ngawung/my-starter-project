#define RLGL_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_ES2

#include <iostream>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include "android_native_app_glue.h"
#include "backends/imgui_impl_android.h"
#include "imgui_impl_opengl3.h"

#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

void mainLoop(void);

extern "C" struct android_app *GetAndroidApp(void);

static int32_t handleInputEvent(struct android_app* app, AInputEvent* inputEvent)
{
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(0, 0, "Starter Raylib");

	// rlImGuiSetup(true);
    GetAndroidApp()->onInputEvent = handleInputEvent;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();
	ImGui_ImplAndroid_Init(GetAndroidApp()->window);
	ImGui_ImplOpenGL3_Init("#version 300 es");

	ImFontConfig font_cfg;
	font_cfg.SizePixels = 22.0f;
	io.Fonts->AddFontDefault(&font_cfg);
	ImGui::GetStyle().ScaleAllSizes(3.0f);

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        mainLoop();
    }
#endif

	// rlImGuiShutdown();
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

	CloseWindow();
	return 0;
}

void mainLoop(void)
{
	ImGuiIO& io = ImGui::GetIO();

	BeginDrawing();
	{
		ClearBackground(RAYWHITE);
		DrawText("Hello World!", 20, 30, 24, BLACK);
		DrawText(TextFormat("%i %i, %i %i", GetScreenWidth(), GetScreenHeight(), ANativeWindow_getWidth(GetAndroidApp()->window), ANativeWindow_getHeight(GetAndroidApp()->window)), 20, 50, 24,BLACK);
		// ImVec2 p = ImGui::GetCursorPos();
		// DrawText(TextFormat("%i %i", (int)p.x, (int)p.y), 20, 80, 24,BLACK);
		DrawFPS(0, 0);

		// rlImGuiBegin();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplAndroid_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::ShowDemoWindow();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// rlImGuiEnd();
	}
	EndDrawing();
}
