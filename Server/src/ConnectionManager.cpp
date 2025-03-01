#include "ConnectionManager.h"

#include <thread>
#include <sstream>

#include "library.h"

constexpr int POLL_WAIT			= 100;
constexpr int DISCONNECT_DELAY	= 3;

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
		std::cerr << "ERROR: Failed to create ClientSocket" << std::endl;
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
	this->m_shutdown = false;
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

void ConnectionManager::createChatroom(Client c1, Client c2) {
	
}

void ConnectionManager::await() {
	// Print await info
	std::stringstream ss;
	ss << std::this_thread::get_id();
	library::print("Server checking connections on thread " + ss.str() + '\n');

	// Setup accepting
    sockaddr_in clientAddr;
    int length = sizeof(clientAddr);
    int newClient;
    struct pollfd fds[1];
    fds[0].fd = this->m_serverSocket;
    fds[0].events = POLLIN;
	
    // Checks for client connections
	Client* firstClient = nullptr;
    while (this->isRunning()) {
		if (Poll(fds, 1, POLL_WAIT) > 0) {
			if ((newClient = accept(this->m_serverSocket, (sockaddr*)&clientAddr, &length)) != -1) {
				library::print("Client connected\n");
				// Add first client to queue
				if (firstClient == nullptr) {
					firstClient = new Client(newClient, clientAddr, length);
				}
				// Create chatroom with both clients
				else {
					Client secondClient(newClient, clientAddr, length);
					this->createChatroom(*firstClient, secondClient);
					delete firstClient;
					firstClient = nullptr;
				}
            }
        }
    }
}

void ConnectionManager::countdown() {
	// Countdown math
	clock_t start = clock();
	clock_t cur = clock();
	while (((cur - start) / CLOCKS_PER_SEC) < DISCONNECT_DELAY) {
		cur = clock();
	}
	this->m_isRunning = false;
}

void ConnectionManager::disconnect() {
	// Already shutting down
	if (this->m_shutdown) {
		// Check instant shutdown
		library::print("Would you like to shutdown the server immediately?\n"
			"Type \"yes\" to confirm: ");
		std::string input;
		std::cin >> input;
		if (input.compare("yes") == 0) {
			library::print("Shutting down immediately\n");
			this->m_isRunning = false;
		}
		return;
	}

	// Begins shutdown
	std::string text = "\nThe server will shutdown in " + std::to_string(DISCONNECT_DELAY) + " seconds\n\n";
	library::print(text);
	std::thread t(countdown, this);
	t.detach();
	this->m_shutdown = true;
}

