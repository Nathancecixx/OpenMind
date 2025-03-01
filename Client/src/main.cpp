#define RAYGUI_IMPLEMENTATION

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raygui.h>

#include "NetworkManager.hpp"
#include "MessageManager.hpp"

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, "Middle Ground");

	MessageManager mm;

	NetworkManager nm([&](Packet p, bool isOwner) {
        mm.addMessage(p.data(), isOwner);
    });

	nm.initConnection(7771, "192.168.144.171");
	
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		GuiButton({0,0,40,20}, "Button");
		EndDrawing();
	}

	CloseWindow();
	nm.disconnect();

	return 0;
}
