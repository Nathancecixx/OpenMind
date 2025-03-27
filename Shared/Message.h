#pragma once

#include <iostream>

class Message {
public:
    std::string str;
    std::string date;
    bool isOwner;
    bool isWarning;
public:
    Message(std::string s, std::string d, bool isOwner, bool isWarning);
};