#include <iostream>
#include <thread>

#include "ConnectionManager.h"
#include "library.h"

void determineOption(int input, ConnectionManager& connection) {
	switch(input) {
	case 0:
		connection.disconnect();
		break;
	default:
		library::print("Not a valid option\n");
		break;
	}
}

void serverInput(ConnectionManager& connection) {
	while(connection.isRunning()) {
		// 
		library::print("Enter an option\n"
			"0. Close server (has delay)\n"
		);
		int input;

		// Check input
		if (!(std::cin >> input)) {
			std::cin.clear();
			std::cin.ignore();
			library::print("Invalid input\n");
			continue;
		}

		determineOption(input, connection);
	}
}

int main(int argc, char** argv) {
	// Create and check connection
	ConnectionManager connect;
	if (!connect.isSetup()) {
		std::cerr << "Could not create connection...\n";
		return 1;
	}

	// Create thread for awaiting
	std::thread await(&ConnectionManager::await, connect);
	await.detach();

	// Thread for typing on server
	std::thread inputThread(serverInput, std::ref(connect));
	inputThread.detach();
	
	// Run loop while server is running
	while (connect.isRunning()) {
	}

	return 0;
}

