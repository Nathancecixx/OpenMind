#pragma once

#include <iostream>

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#else
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "Client.h"

constexpr int DEFAULT_PORT  = 7771;

class ConnectionManager {
private:
// Windows required socket functions
#ifdef _WIN32
    WSADATA m_wsaData;
    SOCKET m_serverSocket;
#else
    int m_serverSocket;
#endif
    
    // For checking that the server should be running
    bool m_isRunning;
    // Holds that the shutdown signal has been sent
    bool m_shutdown;
    // For checking connection properly setup
    bool m_validSetup;
    // Stores connection socket addr
    sockaddr_in m_addr;

    // ----- Updating ----- Functions -----

    // Countdown for server shutdown
    void countdown();

    // ----- Client ----- Handling -----

    void createChatroom(Client c1, Client c2);
    
public:
    // ----- Creation -----
    ConnectionManager(int port = DEFAULT_PORT);
    bool isSetup();
    ~ConnectionManager();
    
    // Awaits new client connections
    // Threadable
    void await();

    // Main server running
    bool isRunning();
    void disconnect();
};

