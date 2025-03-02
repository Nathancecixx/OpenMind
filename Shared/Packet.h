#pragma once

#include <iostream>

class Packet {
public:
    // Type
    enum TYPE { INVALID, CONNECT, DISCONNECT, SERVER_CLOSE, MESSAGE, PROMPT };

private:
    // Holds the databuffer
    char* m_data;

    // Holds header data
    typedef struct {
        unsigned short data;
    } Header;

    Header m_head;

public:
    // ----- Creation ----- Destruction -----
    
    Packet();
    ~Packet();

    // ----- Data ----- Manipulation -----

    void serialize(int packetNum, TYPE type, const std::string& message);
    void deserialize(char* received);

    // ----- Getters -----

    int flags() const noexcept;
    int type() const noexcept;
    int length() const noexcept;
    char* data() const noexcept;
};

