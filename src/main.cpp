#define RAYGUI_IMPLEMENTATION

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raygui.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, "raylib basic window");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		GuiButton({0,0,40,20}, "Button");
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
