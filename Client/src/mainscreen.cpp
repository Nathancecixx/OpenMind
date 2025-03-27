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
    // Get the current room's prompt
    std::string prompt = GetPrompt();

    // Label for Prompt
    DrawText("Prompt:", 10, 15, 20, WHITE);

    // Draw prompt box with a slight background and outline
    Rectangle promptBox = {10, 50, 200, 300};
    DrawRectangleRec(promptBox, Fade(DARKGRAY, 0.2f)); // Slight background color
    DrawRectangleLinesEx(promptBox, 2, Fade(BLACK, 0.3f)); // Subtle outline
    DrawWrappedText(prompt, promptBox, 20, WHITE);


    // Check if ENTER is pressed to send message
    if (editChatMessageInputField && IsKeyPressed(KEY_ENTER)) {
        if (OnChatMessageSent != nullptr) {
            OnChatMessageSent();
        }
    }

    // Chat input box
    Rectangle chatInputBox = {224, 650, 512, 64};
    // Highlight when active
    if (editChatMessageInputField) DrawRectangleLinesEx(chatInputBox, 3, BLUE);
    //
    if (GuiTextBox(chatInputBox, currentChatMessage, 1000, editChatMessageInputField)) {
        editChatMessageInputField = !editChatMessageInputField;
    }
    
    // Draw shadow for the send button
    DrawRectangleRec({768 + 5, 650 + 5, 64, 64}, Fade(BLACK, 0.2f));
    // Draw the send button
    if (GuiButton({768, 650, 64, 64}, "Send")) {
        if (OnChatMessageSent != nullptr) {
            OnChatMessageSent();
        }
    }

    // Scrollable chat panel
    GuiScrollPanel({224, 16, 612, 624}, "Chat", {224, 16, 597, contentHeight}, &chatScroll, &chatView);

    // Check if there are messages to display
    if (GetMessages == nullptr) return;
    
    contentHeight = 0;
    std::vector<Message> messages = GetMessages();
    if (messages.empty()) return;

    // Begin chat message rendering with scissor mode for clipping
    BeginScissorMode(chatView.x, chatView.y, chatView.width, chatView.height);
    
    int chatXOffset = 10;
    int chatYOffset = 16;
    int bubblePadding = 10;
    int spacing = 48;
    Color userColor = {50, 150, 255, 255};  // Blue for sent messages
    Color receivedColor = {200, 200, 200, 255}; // Light gray for received messages
    Color warningColor = {255, 165, 0, 255}; // Orange for warning messages


    int maxBubbleWidth = chatView.width * 0.8;
    int fontSize = 18;
    int lineHeight = fontSize + 4;

    for (size_t i = 0; i < messages.size(); i++) {
        //Get the current bubbles color
        Color bgColor = (messages[i].isOwner)
        ? (messages[i].isWarning ? warningColor : userColor)
        : receivedColor;

        //Measure the current messages text
        int textWidth = MeasureText(messages[i].str.c_str(), 18) + bubblePadding * 2;
        
        //Array of strings for each line of the wrapping
        std::vector<std::string>wrappedText;

        //If the message is too big wrap it, 
        //if not just add it to the array alone to display
        if(textWidth > maxBubbleWidth){
            wrappedText = WrapText(messages[i].str, maxBubbleWidth, fontSize);
            textWidth = maxBubbleWidth;
        } else {
            wrappedText.push_back(messages[i].str);
        }

        //Get the height of the bubble
        int bubbleHeight = wrappedText.size() * lineHeight + 10;
        
        //Determine left or right align in view
        int bubbleX = messages[i].isOwner 
        ? chatView.x + chatView.width - textWidth - chatXOffset                                    
        : chatView.x + chatXOffset;

        //Creat bubble rectangle
        Rectangle bubbleRect = {bubbleX, chatView.y + chatScroll.y + contentHeight + 10, textWidth, bubbleHeight};

        //Draw chat bubble
        DrawRectangleRounded(bubbleRect, 0.5f, 10, bgColor);
        //Draw each line in the wrapped text array
        for (size_t j = 0; j < wrappedText.size(); j++) {
            DrawText(wrappedText[j].c_str(), bubbleRect.x + bubblePadding, bubbleRect.y + 5 + (j * lineHeight), fontSize, BLACK);
        }

        contentHeight += bubbleHeight + 10;
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