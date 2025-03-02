#define RAYGUI_IMPLEMENTATION

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raygui.h>

#include "NetworkManager.hpp"
#include "MessageManager.hpp"
#include "ui/mainscreen.h"

int main(void) {
	const int screenWidth = 960;
	const int screenHeight = 720;
	InitWindow(screenWidth, screenHeight, "Open Mind");

	MessageManager mm;

	NetworkManager nm([&](Packet p, bool isOwner) {
        mm.addMessage(p.data(), isOwner);
    });

	MainScreen mainScreen = MainScreen();

	mainScreen.OnChatMessageSent = [&]() {
		nm.sendMessage(mainScreen.GetCurrentChatMessage());
	};

	mainScreen.OnStartChattingButtonClicked = [&]() {
		bool success = nm.initConnection(7771, "192.168.144.171");
		if (success) {
			mainScreen.GoToChatMenu();
		}
	};

	mainScreen.GetMessages = [&]() {
		return mm.getMessages();
	};
	
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
		
		mainScreen.Draw();

		EndDrawing();
	}

	CloseWindow();
	nm.disconnect();

	return 0;
}
