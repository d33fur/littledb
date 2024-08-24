#include "client_handler.h"
#include "database.h"
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>

void handleClient(int clientSocket, Database& db) {
    char buffer[1024];
    std::string response;

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            break;
        }
        buffer[bytesReceived] = '\0';
        std::string request(buffer);

        request.erase(request.find_last_not_of(" \n\r\t") + 1);

        std::istringstream iss(request);
        std::string command, key, value;
        iss >> command >> key;
        
        if (command == "SET" || command == "set") {
            std::getline(iss, value);
            value.erase(0, value.find_first_not_of(" "));
            db.set(key, value);
            response = "OK\n";

            std::cout << "SET command: key = " << key << ", value = " << value << std::endl;

        } else if (command == "GET" || command == "get") {
            auto value = db.get(key);
            if (value) {
                response = *value + "\n";
            } else {
                response = "(nil)\n";
            }

            std::cout << "GET command: key = " << key << ", value = " << (value ? *value : "nil") << std::endl;

        } else if (command == "EXIT" || command == "exit") {
            response = "Goodbye!\n";
            send(clientSocket, response.c_str(), response.length(), 0);
            break;
        } else if (command == "BACKUP") {
            response = "Backup completed\n";
        } else {
            response = "Unknown command\n";
        }

        send(clientSocket, response.c_str(), response.length(), 0);
    }

    close(clientSocket);
}
