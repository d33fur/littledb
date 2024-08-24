#include "config.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

std::string Config::serverIP_ = "127.0.0.1";
int Config::serverPort_ = 6378;
std::string Config::databaseFile_ = "/var/lib/littledb/data.db";
std::string Config::backupFile_ = "/var/lib/littledb/data_backup.db";
std::string Config::logFile_ = "/var/log/littledb/server.log";
std::string Config::logLevel_ = "INFO";

void Config::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open config file: " << filename << std::endl;
        return;
    }

    json configJson;
    file >> configJson;

    serverIP_ = configJson["server"]["ip"].get<std::string>();
    serverPort_ = configJson["server"]["port"].get<int>();
    databaseFile_ = configJson["database"]["file"].get<std::string>();
    backupFile_ = configJson["database"]["backup_file"].get<std::string>();
    logFile_ = configJson["logging"]["log_file"].get<std::string>();
    logLevel_ = configJson["logging"]["log_level"].get<std::string>();
}

std::string Config::getServerIP() {
    return serverIP_;
}

int Config::getServerPort() {
    return serverPort_;
}

std::string Config::getDatabaseFile() {
    return databaseFile_;
}

std::string Config::getBackupFile() {
    return backupFile_;
}

std::string Config::getLogFile() {
    return logFile_;
}

std::string Config::getLogLevel() {
    return logLevel_;
}
