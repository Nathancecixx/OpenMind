#pragma once

#include <iostream>

class Packet {
private:
    // Flags
    enum FLAGS { none };
    // Type
    enum TYPE { invalid, connect, deconnect, server_close, message };
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
    Packet() {
        this->m_data = nullptr;
    }

    ~Packet() {
        delete [] this->m_data;
    }

    // ----- Data ----- Manipulation -----

    void serialize(int flags, int type, const std::string& message) {
        // Add header data
        this->m_head.flags = flags;
        this->m_head.type = type;
        this->m_head.length = message.length();
        memcpy(this->m_data, &this->m_head, sizeof(Header));
        int increment = sizeof(Header);

        // Add body data
        this->m_data = new char[this->m_head.length];
        memcpy((this->m_data + increment), message.c_str(), this->m_head.length);
    }

    void deserialize(char* received) {
        // Get header data
        memcpy(&this->m_head, received, sizeof(Header));
        int increment = sizeof(Header);

        // Add data
        this->m_data = new char[this->m_head.length];
        memcpy(&this->m_data, received + increment, this->m_head.length);
    }

    // ----- Getters -----

    int flags() {
        return this->m_head.flags;
    }

    int type() {
        return this->m_head.type;
    }

    int length() {
        return this->m_head.length;
    }

    char* data() {
        return this->m_data;
    }
};

