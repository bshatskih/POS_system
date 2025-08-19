#pragma once
#include <include/sqlite3.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_map>

using Row = std::unordered_map<std::string, std::string>;
using Rows = std::vector<Row>;

class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    Rows executeQuery(const std::string& sql, const std::vector<std::string>& params = {});
    bool executeUpdate(const std::string& sql, const std::vector<std::string>& params = {});

    // Транзакции
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

private:
    sqlite3* db_;

    void checkOk(int result, const std::string& errorMsg); 
    void checkStep(int result, const std::string& errorMsg);
    void checkDbNotNull() const;
};