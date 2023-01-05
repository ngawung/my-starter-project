#include <iostream>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void mainLoop(void);

int main()
{

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(960, 600, "Starter Raylib");

	rlImGuiSetup(true);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        mainLoop();
    }
#endif

	rlImGuiShutdown();

	CloseWindow();
	return 0;
}

void mainLoop(void)
{
	BeginDrawing();
	{
		ClearBackground(RAYWHITE);
		DrawText("Hello World!", 20, 30, 24, BLACK);
		DrawFPS(0, 0);

		rlImGuiBegin();
		{
			ImGui::ShowDemoWindow();
		}
		rlImGuiEnd();
	}
	EndDrawing();
}
