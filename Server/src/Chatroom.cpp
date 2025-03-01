#include "Chatroom.h"

#include "Packet.h"
#include "library.h"

// ----- Creation ----- Destruction -----

Chatroom::Chatroom(Client c1, Client c2) : m_client1(c1), m_client2(c2) {
    this->chatSend(c1, "Heyo\0");
    this->chatSend(c2, "Heyo\0");
}

Chatroom::~Chatroom() {

}

// ----- Chatroom ----- Functions -----

void Chatroom::echo() {

}

void Chatroom::chatSend(const Client& client, const std::string& message) {
    Packet data;
    data.serialize(Packet::NONE, Packet::MESSAGE, "Heyo");
    if (data.data() == nullptr) {
        std::cout << "No data to send\n";
        return;
    }
    std::cout << data.data() << ", size: " << strlen(data.data()) << "\n";
    send(client.socket(), data.data(), strlen(data.data()), 0);
}

