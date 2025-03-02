#include "ui/mainscreen.h"
#include <raylib/raygui.h>
#include <string.h>

MainScreen::MainScreen() {
    GuiLoadStyle("../assets/style_amber.rgs");
}

void MainScreen::Draw() {
    switch (currentMenu) {
        case Login: DrawLogin(); break;
        case Main: DrawMainMenu(); break;
        case Chat: DrawChat(); break;
    }
}

void MainScreen::DrawLogin() {
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 64);
    GuiLabel({0, 128, 960, 64}, "Welcome to Open Mind!");
    
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel({328, 256, 64, 64}, "Email:");
    if (GuiTextBox({400, 256, 160, 64}, email, 128, editEmailInputField)) {
        editEmailInputField = !editEmailInputField;
    }
    
    GuiLabel({328, 352, 64, 64}, "Password:");
    if (GuiTextBox({400, 352, 160, 64}, passwordView, 128, editPasswordInputField)) {
        editPasswordInputField = !editPasswordInputField;
    }
    
    if (password.length() < strlen(passwordView)) {
        char lastChar = passwordView[strlen(passwordView) - 1];
        password += lastChar;
        passwordView[strlen(passwordView) - 1] = '*';
    } else if (password.length() > strlen(passwordView)) {
        if (password.length() > 0) {
            password.pop_back();
        }
    }

    //std::cout << realPassword << std::endl;

    if (GuiButton({400, 448, 160, 64}, "Login")) {
        if (strlen(email) > 0 && !password.empty()) {
            currentMenu = Main;
        }
    }
}

void MainScreen::DrawMainMenu() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiPanel({16, 16, 256, 384}, "Profile");
    GuiLabel({24, 36, 256, 32}, "Name: John Doe");

    if (GuiButton({624, 608, 320, 96}, "Start Chatting")) {
        if (OnStartChattingButtonClicked != nullptr) {
            OnStartChattingButtonClicked();
        }
    }
}

void MainScreen::DrawChat() {
    if (GuiTextBox({224, 656, 512, 64}, currentChatMessage, 1000, editChatMessageInputField)) {
        editChatMessageInputField = !editChatMessageInputField;
    }

    if (GuiButton({768, 656, 64, 64}, "Send")) {
        if (OnChatMessageSent != nullptr) {
            OnChatMessageSent();
        }
    }

    GuiScrollPanel({224, 16, 512, 624}, "Chat", {224, 16, 497, contentHeight}, &chatScroll, &chatView);

    if (GetMessages == nullptr) {
        return;
    }

    contentHeight = 0;
    std::vector<Message> messages = GetMessages();
    if (messages.empty()) {
        return;
    }
    /*std::vector<Message> messages = std::vector<Message>();
    for (int i = 0; i < 20; i++) {
        messages.push_back(Message("Hello World!", "Today", i % 2 == 0 ? true : false));
    }*/

    BeginScissorMode(chatView.x, chatView.y, chatView.width, chatView.height);
        int spacing = 48;

        for (int i = 0; i < messages.size(); i++) {
            GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, messages[i].isOwner ? TEXT_ALIGN_RIGHT : TEXT_ALIGN_LEFT);
            GuiStatusBar({chatView.x, chatView.y + chatScroll.y + (i * spacing), chatView.width, 32}, messages[i].str.c_str());
            contentHeight += spacing;
        }
    EndScissorMode();
}

std::string MainScreen::GetCurrentChatMessage() {
    return currentChatMessage;
}

void MainScreen::GoToChatMenu() {
    currentMenu = Chat;
}

void MainScreen::GoToMainMenu() {
    currentMenu = Main;
}

void MainScreen::ClearChatInput() {
    memset(currentChatMessage, 0, 1000);
}