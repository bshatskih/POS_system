#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include "sqlite3.h"

// Глобальные переменные для хранения данных о колонках
std::vector<std::string> columns;
std::vector<std::vector<std::string>> rows;
std::vector<size_t> column_widths;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    // Если это первый вызов - сохраняем названия колонок
    if (columns.empty()) {
        for (int i = 0; i < argc; i++) {
            columns.push_back(azColName[i]);
            column_widths.push_back(strlen(azColName[i]));
        }
    }
    
    // Сохраняем данные строки
    std::vector<std::string> row;
    for (int i = 0; i < argc; i++) {
        std::string value = argv[i] ? argv[i] : "NULL";
        row.push_back(value);
        if (value.length() > column_widths[i]) {
            column_widths[i] = value.length();
        }
    }
    rows.push_back(row);
    
    return 0;
}

void print_table() {
    // Вычисляем общую ширину таблицы
    size_t total_width = 1; // начинаем с 1 для правой границы
    for (size_t width : column_widths) {
        total_width += width + 3; // +3 для " | "
    }
    
    // Верхняя граница таблицы
    std::cout << std::string(total_width, '-') << std::endl;
    
    // Заголовки колонок
    std::cout << "|";
    for (size_t i = 0; i < columns.size(); i++) {
        std::cout << " " << std::left << std::setw(column_widths[i]) << columns[i] << " |";
    }
    std::cout << std::endl;
    
    // Граница под заголовками
    std::cout << std::string(total_width, '-') << std::endl;
    
    // Данные строк
    for (const auto& row : rows) {
        std::cout << "|";
        for (size_t i = 0; i < row.size(); i++) {
            std::cout << " " << std::left << std::setw(column_widths[i]) << row[i] << " |";
        }
        std::cout << std::endl;
    }
    
    // Нижняя граница таблицы
    std::cout << std::string(total_width, '-') << std::endl;
}

int main() {
    sqlite3* db;
    char *err_msg = nullptr;
    int rc = sqlite3_open("../../SQLite_test_project/new_bd.sqlite", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // ... (остальной код создания и заполнения таблицы остается таким же)

    const char* sql_SELECT = "SELECT * FROM Teacher";
    rc = sqlite3_exec(db, sql_SELECT, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (SELECT): " << err_msg << std::endl;
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Выводим таблицу
    print_table();

    std::cout << "Database operations completed successfully!" << std::endl;

    sqlite3_close(db);
    return 0;
}