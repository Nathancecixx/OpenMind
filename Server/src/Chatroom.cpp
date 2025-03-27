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

void Chatroom::warningSend(const Client& client){
    Packet data;
    data.serialize(0, Packet::WARNING, "Warning");
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

    int count = 0;
    bool isProgressed = false;

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

                //count if not yet progressed
                if(!isProgressed) count++;
            }
        }

        //If progressed just continue
        if(isProgressed) {
            continue;
        } else if(count >= 5) {
            //message count hit, send second prompt
            for(Client c : this->m_clients){
                this->promptSend(c, prompt.second);
                isProgressed = true;
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

    std::string sentiment = this->analyzeSentiment(packet.data());
    std::cout << "Sentimate: " << sentiment << std::endl;

    if(sentiment.compare("negative") == 0) {
        warningSend(client);
    } else {
        echo(client, packet.data());
    }
}


std::string Chatroom::analyzeSentiment(const std::string& message) {
    std::string script_path = "..\\src\\SentimentAnalysis\\sentiment.py";
    std::string command = "python \"" + script_path + "\" \"" + message + "\"";
    std::string result;
    char buffer[128];

    // Open a process to capture the output of the Python script
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the output from the Python script
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    // Remove trailing newline characters
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

