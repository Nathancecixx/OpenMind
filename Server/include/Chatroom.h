#pragma once

#include "Client.h"

class Chatroom {
private:
    Client m_client1, m_client2;

    // Echo message to everyone in chatroom
    void echo();

    // Return message to sender
    void rts();

    // Send function
    void chatSend(const Client& client, const std::string& message);

    // Receive function
    void chatRecv();

public:
    // Takes 2 clients into chatroom
    Chatroom(Client c1, Client c2);

    ~Chatroom();
};

