#include "Chatroom.h"

// ----- Creation ----- Destruction -----

Chatroom::Chatroom(Client c1, Client c2) : m_client1(c1), m_client2(c2) {
    this->chatSend(c1, "Heyo\0");
}

Chatroom::~Chatroom() {

}

// ----- Chatroom ----- Functions -----

void Chatroom::echo() {

}

void Chatroom::chatSend(const Client& client, const std::string& message) {
    send(client.socket(), message.c_str(), message.length(), 0);
    send(client.socket(), message.c_str(), message.length(), 0);
}

