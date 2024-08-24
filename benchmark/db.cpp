#include <iostream>
#include <chrono>
#include <string>
#include "database.h"

int main() {
    Database db("benchmark.db");

    const int numOperations = 100000;
    const std::string value = "value";

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i) {
        db.set("key" + std::to_string(i), value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> setDuration = end - start;
    std::cout << "Time for " << numOperations << " SET operations: " << setDuration.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i) {
        db.get("key" + std::to_string(i));
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> getDuration = end - start;
    std::cout << "Time for " << numOperations << " GET operations: " << getDuration.count() << " seconds\n";

    return 0;
}
