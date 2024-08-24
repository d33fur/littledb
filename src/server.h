#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "database.h"

class Server {
public:
    Server(const std::string& configFilePath);
    void start();
    void backupData();

private:
    void loadConfig(const std::string& configFilePath);
    void setupServerSocket();
    int serverSocket_;
    Database db_;
};

#endif // SERVER_H
