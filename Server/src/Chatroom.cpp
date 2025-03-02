#include "Chatroom.h"

#include <thread>
#include <sstream>
#include <Python.h>
#include "Defines.h"
#include "library.h"

// ----- Creation ----- Destruction -----

Chatroom::Chatroom(Client c1, Client c2) {
    this->m_clients.push_back(c1);
    this->m_clients.push_back(c2);
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

double analyzeSentiment(const std::string& text) {
    Py_Initialize();
    
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
    
    // Load the Python script 'func.py' (without .py extension)
    pName = PyUnicode_DecodeFSDefault("func");
    pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule) {
        // Get the function 'get_compound_score' from func.py
        pFunc = PyObject_GetAttrString(pModule, "get_compound_score");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Create arguments for the function
            pArgs = PyTuple_Pack(1, PyUnicode_FromString(text.c_str()));
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_XDECREF(pArgs);
            
            if (pValue) {
                double result = PyFloat_AsDouble(pValue);
                Py_XDECREF(pValue);
                Py_XDECREF(pModule);
                Py_Finalize();
                return result;
            }
        }
    }

    Py_XDECREF(pModule);
    Py_Finalize();
    return 0.0;  // Default return on failure
}
void Chatroom::checkData(const Client& client, const Packet& packet) {
    std::stringstream data; 
    data << "Type: " << packet.type() << ", Data: " << packet.data() << ", Flags: " << packet.flags() << ", Length: " << packet.length() << '\n';
    library::print(data.str());
    double sentimentScore = analyzeSentiment(packet.data());

    constexpr double NEGATIVE_THRESHOLD = -0.7;

    if (sentimentScore < NEGATIVE_THRESHOLD)
    {
        std::string warnMsg = "Your message was not sent because it was not civil";
        std::cout << warnMsg << std::endl;
    }
    else
    {
        echo(client, packet.data());
    }
}

