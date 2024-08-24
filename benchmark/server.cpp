#include <iostream>
#include <chrono>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class ServerBenchmark {
public:
    ServerBenchmark(const std::string& serverIP, int serverPort) {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            throw std::runtime_error("Could not create socket");
        }

        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            throw std::runtime_error("Could not connect to server");
        }
    }

    ~ServerBenchmark() {
        close(sockfd);
    }

    void benchmark(int numOperations) {
        const std::string value = "value";

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numOperations; ++i) {
            sendCommand("SET key" + std::to_string(i) + " " + value);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> setDuration = end - start;
        std::cout << "Time for " << numOperations << " SET operations via server: " << setDuration.count() << " seconds\n";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numOperations; ++i) {
            sendCommand("GET key" + std::to_string(i));
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> getDuration = end - start;
        std::cout << "Time for " << numOperations << " GET operations via server: " << getDuration.count() << " seconds\n";
    }

private:
    int sockfd;
    struct sockaddr_in serverAddr;

    void sendCommand(const std::string& command) {
        std::string fullCommand = command + "\n";
        send(sockfd, fullCommand.c_str(), fullCommand.length(), 0);

        char buffer[1024] = {0};
        recv(sockfd, buffer, 1024, 0);
    }
};

int main() {
    try {
        ServerBenchmark benchmark("127.0.0.1", 6378);
        benchmark.benchmark(100000);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
