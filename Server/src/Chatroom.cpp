#include "Chatroom.h"

#include <thread>
#include <sstream>
#include <string>

#include "Defines.h"
#include "library.h"

// ----- Creation ----- Destruction -----

Chatroom::Chatroom(Client c1, Client c2) : prompt("", "") {
    // Add clients to room
    this->m_clients.push_back(c1);
    this->m_clients.push_back(c2);

    // Initialize prompt generator and load prompts
    PromptGenerator pg("../Resources/Prompts.txt");
    if (!pg.LoadPrompts()) {
        std::cerr << "Failed to load prompts from file!" << std::endl;
        //Default Prompt
        this->prompt = Prompt(
            "Should social media companies regulate speech, or does that infringe on free speech?", 
            "Can we agree that some balance is needed between free speech and preventing harm? What measures could work for both sides?"); 
    } else {
        this->prompt = pg.GetRandomPrompt();
    }

    // Send clients the prompt
    this->promptSend(c1, prompt.first); 
    this->promptSend(c2, prompt.first);
}

Chatroom::~Chatroom() {

}

void Chatroom::chat() {
    // Create receive thread
    std::thread receiveThread(&Chatroom::chatRecv, this);
    receiveThread.detach();

    // Print chatroom info
	std::stringstream ss;
	ss << std::this_thread::get_id();
	library::print("\nChatroom created on thread " + ss.str() + '\n');

    while (true) {

    }
}

// ----- Chatroom ----- Functions -----

void Chatroom::echo(const Client& sentFrom, const std::string& message) {
    for (Client c : this->m_clients) {
        if (c.socket() != sentFrom.socket()) {
            this->chatSend(c, message);
        }
    }
}

void Chatroom::chatSend(const Client& client, const std::string& message) {
    Packet data;
    data.serialize(0, Packet::MESSAGE, message);
    if (data.data() == nullptr) {
        std::cout << "No data to send\n";
        return;
    }
    send(client.socket(), data.data(), strlen(data.data()), 0);
}

void Chatroom::promptSend(const Client& client, const std::string& prompt){
    Packet data;
    data.serialize(0, Packet::PROMPT, prompt);
    if (data.data() == nullptr) {
        std::cout << "No data to send\n";
        return;
    }
    send(client.socket(), data.data(), strlen(data.data()), 0);
}

void Chatroom::chatRecv() {
    // Print thread data
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string threadId = ss.str();
    library::print("Receive thread created on thread " + threadId + '\n');

    // Get receive data
    char* recvBuf;
    while (true) {
        for (Client c : this->m_clients) {
            if ((recvBuf = this->pollRecv(c.socket())) != nullptr) {
                if (strcmp(recvBuf, "end of message") == 0) {
                    library::print("Exiting room with thread " + threadId);
                    return;
                }

                // Packet info
                Packet p;
                p.deserialize(recvBuf);
                recvBuf = nullptr;
                this->checkData(c, p);
            }
        }
    }
}

char* Chatroom::pollRecv(int fd) {
    static struct pollfd fds[1];
    fds[0].events = POLLIN;
    fds[0].fd = fd;
    static char RxBuffer[MAX_RECV];

    if (Poll(fds, 1, POLL_WAIT / this->m_clients.size())) {
        int size = recv(fd, RxBuffer, MAX_RECV, 0);
        if (size <= 0) {
            return (char*)"end of message";
        }
        return RxBuffer;
    }

    return nullptr;
}

void Chatroom::checkData(const Client& client, const Packet& packet) {
    std::stringstream data; 
    data << "Type: " << packet.type() << ", Data: " << packet.data() << ", Flags: " << packet.flags() << ", Length: " << packet.length() << '\n';
    library::print(data.str());
    echo(client, packet.data());
}

