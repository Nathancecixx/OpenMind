#include "ui/mainscreen.h"
#include <raylib/raygui.h>

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

    //Draw Shadow
    DrawRectangleRec({400 + 5, 448 + 5, 160, 64}, Fade(BLACK, 0.2f));
    //Draw Button
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

    DrawRectangleRec({624 + 5, 608 + 5, 320, 96}, Fade(BLACK, 0.2f));
    if (GuiButton({624, 608, 320, 96}, "Start Chatting")) {
        if (OnStartChattingButtonClicked != nullptr) {
            OnStartChattingButtonClicked();
        }
    }
}

void MainScreen::DrawChat() {

    //Get the current rooms prompt
    std::string prompt = GetPrompt();

    GuiLabel({10, 10, 256, 32}, "Prompt:");

    Rectangle promptBox = {10, 50, 200, 300};
    // Draw a box
    DrawRectangleRec(promptBox, LIGHTGRAY);
    DrawRectangleLines(promptBox.x, promptBox.y, promptBox.width, promptBox.height, DARKGRAY);

    // Draw wrapped text inside the box
    DrawWrappedText(prompt, promptBox, 20, BLACK);

    if (GuiTextBox({224, 656, 512, 64}, currentChatMessage, 1000, editChatMessageInputField)) {
        editChatMessageInputField = !editChatMessageInputField;
    }

    //Draw Shadow
    DrawRectangleRec({768 + 5, 656 + 5, 64, 64}, Fade(BLACK, 0.2f));
    //Draw Button
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

// Function to wrap text within a specified width
std::vector<std::string> MainScreen::WrapText(const std::string& text, int maxWidth, int fontSize) {
    std::vector<std::string> lines;
    std::istringstream words(text);
    std::string word, currentLine;
    
    while (words >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;

        // Check if the new line would exceed the max width
        if (MeasureText(testLine.c_str(), fontSize) > maxWidth) {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
            }
            currentLine = word; // Start a new line with the current word
        } else {
            currentLine = testLine;
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

void MainScreen::DrawWrappedText(const std::string& text, Rectangle box, int fontSize, Color color) {
    std::vector<std::string> lines = WrapText(text, box.width, fontSize);
    int lineSpacing = fontSize + 4; // Adjust line spacing

    for (size_t i = 0; i < lines.size(); ++i) {
        DrawText(lines[i].c_str(), box.x + 5, box.y + 5 + (i * lineSpacing), fontSize, color);
    }
}