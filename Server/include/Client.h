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

class Client {
private:
    int m_socket, m_sockLength;
    sockaddr_in m_addr;

public:
    Client(const Client& c);
    Client(int socket, sockaddr_in addr, int length);
    ~Client();

    int socket() const noexcept;
    int socketLength() const noexcept;
    sockaddr_in addr() const noexcept;
};

