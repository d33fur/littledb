#include "database.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "config.h"

Database::Database(const std::string& dbFile) : dbFile_(dbFile) {
    loadFromFile();
}

Database::~Database() {
    saveToFile();
}

void Database::set(const std::string& key, const std::string& value) {
    storage_[key] = value;
}

std::optional<std::string> Database::get(const std::string& key) const {
    auto it = storage_.find(key);
    if (it != storage_.end()) {
        return it->second;
    }
    return std::nullopt;
}

void Database::del(const std::string& key) {
    storage_.erase(key);
}

void Database::loadFromFile() {
    std::ifstream file(dbFile_);
    std::string key, value;

    while (std::getline(file, key) && std::getline(file, value)) {
        storage_[key] = value;
    }
}

void Database::saveToFile() const {
    std::ofstream file(dbFile_);
    for (const auto& [key, value] : storage_) {
        file << key << "\n" << value << "\n";
    }
}

void Database::backupData() const {
    std::filesystem::copy_file(dbFile_, Config::getBackupFile(), std::filesystem::copy_options::overwrite_existing);
}
