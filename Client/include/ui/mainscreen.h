#pragma once

#include <raylib/raylib.h>
#include <iostream>
#include <vector>
#include <functional>
#include "Message.h"

class MainScreen {
private:
    char email[128] = { 0 };
    char passwordView[128] = { 0 };
    std::string password = "";

    char currentChatMessage[1000] = { 0 };

public:
    MainScreen();

    typedef enum Menu {
        Login,
        Main,
        Chat
    } Menu;

    Menu currentMenu = Login;

    bool editEmailInputField = false;
    bool editPasswordInputField = false;
    bool editChatMessageInputField = false;
    Vector2 chatScroll = { 0 };
    Rectangle chatView = { 0 };
    float contentHeight = 64.0f;

    void Draw();
    void DrawLogin();
    void DrawMainMenu();
    void DrawChat();

    void GoToChatMenu();
    void GoToMainMenu();
    std::string GetCurrentChatMessage();
    void ClearChatInput();

    std::function<void()> OnStartChattingButtonClicked = nullptr;
    std::function<void()> OnChatMessageSent = nullptr;
    std::function<std::vector<Message>()> GetMessages = nullptr;
    std::function<std::string()> GetPrompt = nullptr;
};