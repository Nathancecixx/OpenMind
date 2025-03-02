#pragma once

#include <iostream>
#include <vector>

#include "Client.h"
#include "Packet.h"
#include "PromptGenerator.h"

class Chatroom {
private:
    std::vector<Client> m_clients;

    Prompt prompt;

    // Echo message to everyone in chatroom
    void echo(const Client& sentFrom, const std::string& message);

    // Return message to sender
    void rts(const Client& sentFrom, const std::string& message);

    // Send function
    void chatSend(const Client& client, const std::string& message);

    // Send function
    void promptSend(const Client& client, const std::string& prompt);

    // Receive function
    void chatRecv();

    // Polling an fd
    char* pollRecv(int fd);

    // Check received data
    void checkData(const Client& client, const Packet& packet);

public:
    // Takes 2 clients into chatroom
    Chatroom(Client c1, Client c2);

    void chat();

    ~Chatroom();
};

