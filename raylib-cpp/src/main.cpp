#include <iostream>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

int main()
{
	std::cout << "hello world\n";

	InitWindow(512, 512, "Starter Raylib");
	SetTargetFPS(60);

	rlImGuiSetup(true);

	while (!WindowShouldClose())
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

	rlImGuiShutdown();

	return 0;
}
