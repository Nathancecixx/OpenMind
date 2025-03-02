#include "MessageManager.hpp"

bool MessageManager::addMessage(std::string str, bool isOwner){
    std::string date = getCurrentTime();
    Message message(str, date, isOwner);

    //Lock mutex before vector edit
    std::lock_guard<std::mutex> lock(messageListMutex);
    this->messageList.push_back(message);
    return true;
}

bool MessageManager::addPrompt(std::string str){
    this->prompt = str;
    return true;
}

std::vector<Message> MessageManager::getMessages(){
    //Lock mutex before vector edit
    return messageList;
}

std::string MessageManager::getPrompt(){
    return prompt;
}

void MessageManager::resetList(){
    //Lock mutex before vector edit
    messageList.clear();
}

std::string MessageManager::getCurrentTime(){
    // Get current time as time_t
    auto now = std::time(nullptr);

    // Convert to tm structure for local time
    std::tm localTime{};
    localtime_s(&localTime, &now);    
    
    // Create a string stream and format the time
    std::ostringstream timeStream;
    timeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return timeStream.str();
}