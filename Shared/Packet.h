#pragma once

#include <iostream>

class Packet {
public:
    // Flags
    enum FLAGS { NONE };
    // Type
    enum TYPE { INVALID, CONNECT, DECONNECT, SERVER_CLOSE, MESSAGE };

private:
    // Holds the databuffer
    char* m_data;

    // Holds header data
    typedef struct {
        unsigned short flags : 3;
        unsigned short type : 3;
        unsigned short length : 10;
    } Header;

    Header m_head;

public:
    // ----- Creation ----- Destruction -----
    
    Packet();
    ~Packet();

    // ----- Data ----- Manipulation -----

    void serialize(FLAGS flags, TYPE type, const std::string& message);
    void deserialize(char* received);

    // ----- Getters -----

    int flags();
    int type();
    int length();
    char* data();
};

