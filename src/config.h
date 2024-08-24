#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    static void loadConfig(const std::string& filename);

    static std::string getServerIP();
    static int getServerPort();
    static std::string getDatabaseFile();
    static std::string getBackupFile();
    static std::string getLogFile();
    static std::string getLogLevel();
    
private:
    static std::string serverIP_;
    static int serverPort_;
    static std::string databaseFile_;
    static std::string backupFile_;
    static std::string logFile_;
    static std::string logLevel_;
};

#endif // CONFIG_H
