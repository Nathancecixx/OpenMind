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

public:
    MessageManager() = default;

    bool addMessage(std::string message, bool isOwner);

    std::vector<Message> getMessages();

    void resetList();

private:
    std::string getCurrentTime();

};