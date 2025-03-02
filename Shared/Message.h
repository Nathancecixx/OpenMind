#pragma once

class Message {
public:
    std::string str;
    std::string date;
    bool isOwner;
public:
    Message(std::string s, std::string d, bool isOwner) 
    : str(s), date(d), isOwner(isOwner) {}
};