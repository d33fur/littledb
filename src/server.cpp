#include "server.h"
#include "config.h"
#include "client_handler.h"
#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

Server::Server(const std::string& configFilePath) : db_(Config::getDatabaseFile()) {
    Config::loadConfig(configFilePath);
}

void Server::setupServerSocket() {
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        throw std::runtime_error("Cannot create socket");
    }

    sockaddr_in serverAddr {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(Config::getServerIP().c_str());
    serverAddr.sin_port = htons(Config::getServerPort());

    if (bind(serverSocket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(serverSocket_, 3) < 0) {
        throw std::runtime_error("Listen failed");
    }
}

void Server::start() {
    setupServerSocket();

    while (true) {
        int clientSocket = accept(serverSocket_, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::thread([this, clientSocket]() { handleClient(clientSocket, db_); }).detach();
    }
}

void Server::backupData() {
    db_.backupData();
}
