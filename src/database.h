#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <optional>

class Database {
public:
    Database(const std::string& dbFile);
    ~Database();

    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    void del(const std::string& key);
    void backupData() const;

private:
    std::unordered_map<std::string, std::string> storage_;
    std::string dbFile_;

    void loadFromFile();
    void saveToFile() const;
};

#endif // DATABASE_H
