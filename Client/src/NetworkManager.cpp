#include <iostream>
#include "NetworkManager.hpp"

NetworkManager::NetworkManager(std::function<void(std::string, bool)> messageCallback, std::function<void(std::string)> promptCallback) {
    onMessage = messageCallback;
    onPrompt = promptCallback;
}

bool NetworkManager::initConnection(int Port, const char* ip){
    //WSA Startup
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return false;
    }    

    //Create the socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET){
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    //Create the server addr
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    //Connect to server
    if(connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR){
        std::cerr << "Failed to connect to server over socket." << std::endl;
        return false;
    }

    //Set server sate to running
    running = true;
    //Start thread to recieve messages
    receiverThread = std::thread(&NetworkManager::recieveMessages, this);
    return true;
}

bool NetworkManager::sendMessage(std::string message){
    //Create a new packet and serialize 
    Packet packet;
    packet.serialize(0, Packet::MESSAGE, message);
    
    if(packet.data() == nullptr) {
        std::cerr << "Failed to serialize packet" << std::endl;
        return false;
    }

    //Send serialized data over socket
    int bytes_sent = send(sock, packet.data(), strlen(packet.data()), 0);
    if(bytes_sent <= 0){
        std::cout << "Failed to send bytes" << std::endl;
        return false;
    }

    //std::cout << "Sent: " << packet.data() << std::endl;

    if (packet.data() != nullptr && onMessage != nullptr) {
        onMessage(message, true);
    }
    return true;
}

void NetworkManager::recieveMessages(){
    //Loop while server is running
    while(running){
        //Recieve over connection into buffer 
        char buffer[MAX_MESSAGE_LEN] = {0};
        int bytes_recieved = recv(sock, buffer, sizeof(buffer), 0);
        
        //Check amount recieved
        if(bytes_recieved <= 0){
            std::cout << "Failed to recieve bytes" << std::endl;
            running = false;
            break;
        } 

        //std::cout << "Recieved Raw: " << buffer << std::endl;

        //Create packet and deserialize the buffer
        Packet packet;
        packet.deserialize(buffer);
        
        std::cout << "Recieved Serialized: " << packet.data() << std::endl;
        
        if(packet.data() == nullptr) {
            std::cerr << "Failed to deserialize packet" << std::endl;
            return;
        }

        if (onMessage == nullptr || onPrompt == nullptr) {
            std::cerr << "Failed to access function pointer's" << std::endl;
            return;
        }

        //Parse packet type
        if(packet.type() == Packet::MESSAGE){
            onMessage(packet.data(), false);
        }else if(packet.type() == Packet::PROMPT){
            onPrompt(packet.data());
        }
        memset(buffer, 0, 1000);
    }
};


bool NetworkManager::disconnect(){
    running = false;
    return closesocket(sock) == 0;
}