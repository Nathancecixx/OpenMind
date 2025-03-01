#pragma once

class Message {
private:
    std::string str;
    std::string date;
    bool isOwner;
public:
    Message(std::string s, std::string d, bool isOwner) 
    : str(s), date(d), isOwner(isOwner) {}
};