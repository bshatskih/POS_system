#include <include/db/Database.h>







Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db_)));
    }
}


Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}


void Database::checkDbNotNull() const {
    if (!db_) {
        throw std::runtime_error("Database connection is not open");
    }
}


void Database::checkOk(int result, const std::string& errorMsg) {
    if (result != SQLITE_OK) {
        throw std::runtime_error(errorMsg + ": " + sqlite3_errmsg(db_));
    }
}


Rows Database::executeQuery(const std::string& sql, const std::vector<std::string>& params) {
    checkDbNotNull();
    sqlite3_stmt* stmt = nullptr;
    Rows rows;

    checkOk(sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr), 
          "Failed to prepare statement");

    try {
        for (int i = 0; i < params.size(); ++i) {
            checkOk(sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT),
                  "Failed to bind parameter");
        }

        while (true) {
            int rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                Row row;
                int colCount = sqlite3_column_count(stmt);
                for (int i = 0; i < colCount; ++i) {
                    const char* colName = sqlite3_column_name(stmt, i);
                    const char* colValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                    row[colName] = colValue ? colValue : "NULL";
                }
                rows.push_back(row);
            } else if (rc == SQLITE_DONE) {
                break;
            } else {
                throw std::runtime_error(std::string("Failed to execute query") + ": " + sqlite3_errmsg(db_));
            }
        }
    } catch (...) {
        sqlite3_finalize(stmt);
        throw;
    }

    sqlite3_finalize(stmt);
    return rows;
}



bool Database::executeUpdate(const std::string& sql, const std::vector<std::string>& params) {
    checkDbNotNull();
    sqlite3_stmt* stmt = nullptr;
    
    checkOk(sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr),
          "Failed to prepare statement");

    try {
        for (int i = 0; i < params.size(); ++i) {
            checkOk(sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT),
                  "Failed to bind parameter");
        }
    } catch (...) {
        sqlite3_finalize(stmt);
        throw;
    }

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        return true;
    } else {
        throw std::runtime_error(std::string("Failed to execute update") + ": " + sqlite3_errmsg(db_));
    }
}


void Database::beginTransaction() {
    checkOk(executeUpdate("BEGIN TRANSACTION") ? SQLITE_OK : SQLITE_ERROR,
           "Failed to begin transaction");
}

void Database::commitTransaction() {
    checkOk(executeUpdate("COMMIT") ? SQLITE_OK : SQLITE_ERROR,
           "Failed to commit transaction");
}

void Database::rollbackTransaction() {
    checkOk(executeUpdate("ROLLBACK") ? SQLITE_OK : SQLITE_ERROR,
           "Failed to rollback transaction");
}