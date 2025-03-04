#include "Packet.h"

// ----- Creation ----- Destruction -----
Packet::Packet() {
    this->m_data = nullptr;
}

Packet::~Packet() {
    delete [] this->m_data;
}

// ----- Data ----- Manipulation -----

void Packet::serialize(int packetNum, TYPE type, const std::string& message) {
    // Add header data
    this->m_head.data = 0;
    this->m_head.data |= packetNum << 13;
    this->m_head.data |= type << 10;
    int length = message.length();
    this->m_head.data |= length;
    
    // Setup data
    delete [] this->m_data;
    this->m_data = new char[sizeof(Header) + length];

    // Add header
    memcpy(this->m_data, &this->m_head, sizeof(Header));
    int increment = sizeof(Header);
    
    // Add body data
    memcpy((this->m_data + increment), message.c_str(), length);
}

void Packet::deserialize(char* received) {
    // Get header data
    this->m_head.data = 0;
    memcpy(&this->m_head.data, received, sizeof(Header));
    int increment = sizeof(Header);
    
    // Add data
    delete [] this->m_data;
    int length = this->m_head.data & 0x3ff;
    this->m_data = new char[length + 1];
    memcpy(this->m_data, received + increment, length);
    this->m_data[length] = '\0';
}

// ----- Getters -----

int Packet::flags() const noexcept {
    return ((this->m_head.data >> 13) & 0x7);
}

int Packet::type() const noexcept {
    return ((this->m_head.data >> 10) & 0x7);
}

int Packet::length() const noexcept {
    return (this->m_head.data & 0x3ff);
}

char* Packet::data() const noexcept {
    return this->m_data;
}

