#include <iostream>
#include <raylib.h>

int main()
{
	std::cout << "hello world\n";

	InitWindow(512, 512, "Starter Raylib");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			DrawText("Hello World!", 20, 30, 24, BLACK);
			DrawFPS(0, 0);
		}
		EndDrawing();
	}

	return 0;
}
