#pragma once

#include <mutex>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <vector>
#include <string>

#include "Message.h"


class MessageManager {
private:
    std::vector<Message> messageList;
    std::mutex messageListMutex;

    std::string prompt;


public:
    MessageManager() = default;

    bool addMessage(std::string message, bool isOwner);

    bool addPrompt(std::string message);

    std::vector<Message> getMessages();

    std::string getPrompt();

    void resetList();

private:
    std::string getCurrentTime();

};