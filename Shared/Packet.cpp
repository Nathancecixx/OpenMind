#include "Packet.h"

// ----- Creation ----- Destruction -----
Packet::Packet() {
    this->m_data = nullptr;
}

Packet::~Packet() {
    delete [] this->m_data;
}

// ----- Data ----- Manipulation -----

void Packet::serialize(FLAGS flags, TYPE type, const std::string& message) {
    // Setup data
    delete [] this->m_data;
    int length = sizeof(Header) + this->m_head.length + 1;
    this->m_data = new char[length];

    // Add header data
    this->m_head.flags = flags;
    this->m_head.type = type;
    this->m_head.length = message.length() + 1;

    // Add header
    memcpy(this->m_data, &this->m_head, sizeof(Header));
    int increment = sizeof(Header);
    
    // Add body data
    memcpy((this->m_data + increment), message.c_str() + '\0', this->m_head.length);
}

void Packet::deserialize(char* received) {
    // Get header data
    memcpy(&this->m_head, received, sizeof(Header));
    int increment = sizeof(Header);
    
    // Add data
    delete [] this->m_data;
    this->m_data = new char[this->m_head.length];
    memcpy(this->m_data, received + increment, this->m_head.length);
}

// ----- Getters -----

int Packet::flags() {
    return this->m_head.flags;
}

int Packet::type() {
    return this->m_head.type;
}

int Packet::length() {
    return this->m_head.length;
}

char* Packet::data() {
    return this->m_data;
}

