#include "Client.h"

Client::Client(int socket, sockaddr_in addr, int length) {
    m_socket = socket;
    m_addr = addr;
    m_sockLength = length;
}

Client::Client(const Client& c) {
    this->m_socket = c.m_socket;
    this->m_addr = c.m_addr;
    this->m_sockLength = c.m_sockLength;
}

Client::~Client() {
    // Nothing todo
}

int Client::socket() const noexcept {
    return this->m_socket;
}

int Client::socketLength() const noexcept {
    return this->m_sockLength;
}

sockaddr_in Client::addr() const noexcept {
    return this->m_addr;
}

