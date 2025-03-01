#include "ConnectionManager.h"

#include <thread>

#include "Client.h"

constexpr int POLL_WAIT		= 100;

// ----- Creation ----- Destruction -----

ConnectionManager::ConnectionManager(int port) {
	this->m_validSetup = false;

    // Start windows socket
    #ifdef _WIN32
	    if (WSAStartup(MAKEWORD(2, 2), &this->m_wsaData) != 0) {
	    	std::cerr << "ERROR: Failed to start WSA" << std::endl;
	    	return;
	    }
    #endif

	// Create Socket
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET) {
		std::cerr << "ERORR: Failed to create ClientSocket" << std::endl;
		return;
	}

	// Bind socket
    this->m_addr.sin_family = AF_INET;
    this->m_addr.sin_addr.s_addr = INADDR_ANY;
    this->m_addr.sin_port = htons(port);
    if (bind(this->m_serverSocket, (sockaddr*)&this->m_addr, sizeof(this->m_addr)) == -1) {
        std::cerr << "Failed to bind socket\n";
        return;
    }

    std::cout << "Server bound to port " << port << "\n";

    // Begin listen
    if (listen(this->m_serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen\n";
        return;
    }

	this->m_validSetup = true;
	this->m_isRunning = true;
}

ConnectionManager::~ConnectionManager() {
    #ifdef _WIN32
        closesocket(this->m_serverSocket);
        WSACleanup();
    #else
        close(this->m_serverSocket);
    #endif
}

bool ConnectionManager::isSetup() {
    return this->m_validSetup;
}

bool ConnectionManager::isRunning() {
	return this->m_isRunning;
}

// ----- Updating ----- Server -----

void ConnectionManager::update() {
	
}

void ConnectionManager::await() {
    std::cout << "Server checking connections on thread " << std::this_thread::get_id() << "\n";

    // Checks for client connections
    sockaddr_in clientAddr;
    int length = sizeof(clientAddr);
    int newClient;
    struct pollfd fds[1];
    fds[0].fd = this->m_serverSocket;
    fds[0].events = POLLIN;

    while (this->isRunning()) {
        if (Poll(fds, 1, POLL_WAIT) > 0) {
            if ((newClient = accept(this->m_serverSocket, (sockaddr*)&clientAddr, &length)) != -1) {
				std::cout << "Client connected\n";
				Client c(newClient, clientAddr, length);
            }
        }
    }
}

void ConnectionManager::disconnect() {
	this->m_isRunning = false;
}

void ConnectionManager::messageSend(const std::string& message) {
    // send(this->m_clientSocket, message.c_str(), message.length(), 0);
}

