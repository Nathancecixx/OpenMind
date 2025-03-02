#include "Chatroom.h"

#include <thread>
#include <sstream>

#include "Defines.h"
#include "library.h"

// ----- Creation ----- Destruction -----

Chatroom::Chatroom(Client c1, Client c2) : m_client1(c1), m_client2(c2) {
    this->chatSend(c1, "Heyo");
    this->chatSend(c2, "Heyo");
}

Chatroom::~Chatroom() {

}

void Chatroom::chat() {
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

}

void Chatroom::chatSend(const Client& client, const std::string& message) {
    Packet data;
    data.serialize(Packet::NONE, Packet::MESSAGE, message);
    if (data.data() == nullptr) {
        std::cout << "No data to send\n";
        return;
    }
    send(client.socket(), data.data(), strlen(data.data()), 0);
}

void Chatroom::chatRecv() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    library::print("Receive thread created on thread " + ss.str() + '\n');

    char* recvBuf;
    while (true) {
        // Poll c1
        if ((recvBuf = this->pollRecv(this->m_client2.socket()))) {
            Packet p;
            p.deserialize(recvBuf);
            this->checkData(p);
        }
    }
}

char* Chatroom::pollRecv(int fd) {
    static struct pollfd fds[1];
    fds[0].events = POLLIN;
    fds[0].fd = fd;
    static char RxBuffer[MAX_RECV];

    if (Poll(fds, 1, POLL_WAIT)) {
        recv(fd, RxBuffer, MAX_RECV, 0);
        return RxBuffer;
    }

    return nullptr;
}

void Chatroom::checkData(const Packet& packet) {
    std::stringstream data; data << "Type: " << packet.type();
    int flag = packet.length();
    if (packet.length() == 5) {
        std::cout << "HEYOO!!!\n";
    }
    std::cout << "Check: " << flag << "\n";
    data << ", Data: " << packet.data() << ", Flags: " << packet.flags() << ", Length: " << packet.length() << '\n';
    library::print(data.str());
    Sleep(10000);
}

