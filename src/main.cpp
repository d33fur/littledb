#include "server.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config-file>\n";
        return 1;
    }

    std::string configFilePath = argv[1];
    Server server(configFilePath);
    server.start();
    return 0;
}
