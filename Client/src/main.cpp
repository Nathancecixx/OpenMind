#define RAYGUI_IMPLEMENTATION

#include <iostream>
#include <raylib/raylib.h>
#include <raylib/raygui.h>
#include <raylib/raymath.h>

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

	Camera3D camera = { 0 };
    camera.position = { 0.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 10.0f;
    camera.projection = CAMERA_PERSPECTIVE;
	float timer = 0.0f;
	Model model = LoadModel("../assets/intro.glb");
	float pitch = -22.5;
	float yaw = -80;
	
	while (!WindowShouldClose()) {
		BeginDrawing();
		

		if (timer < 10.0f) {
			ClearBackground({140, 194, 209, 255});
			BeginMode3D(camera);
			yaw -= 27.0f * GetFrameTime();
			model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, 0 });
			DrawModel(model, {0, 0, 0}, 1.0f, WHITE);
			timer += GetFrameTime();
			EndMode3D();

			if (GetKeyPressed() != 0) {
				timer = 10.0f;
			}
		} else {
			ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
			mainScreen.Draw();
		}

		EndDrawing();
	}

	CloseWindow();
	nm.disconnect();

	return 0;
}
