#include "Client.h"

Client::Client(int socket, sockaddr_in addr, int length) {
    m_socket = socket;
    m_addr = addr;
    m_sockLength = length;
}

Client::~Client() {
    
}

