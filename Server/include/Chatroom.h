#pragma once

#include "Client.h"

class Chatroom {
private:
    Client m_client1, m_client2;
    //
public:
    //
    Chatroom();
    Chatroom(Client c1, Client c2);
    ~Chatroom();
};

