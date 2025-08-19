#include <iostream>
#include <include/sqlite3.h>
// #include <include/db/Database.h>
// #include <include/db/CustomerDAO.h>
// #include <include/db/EmployeeDAO.h>
// #include <include/db/ProductDAO.h>
// #include <include/db/SaleDAO.h>
// #include <include/logic/CustomerService.h>
// #include <include/logic/EmployeeService.h>
// #include <include/logic/ProductService.h>
// #include <include/logic/SaleService.h>
// #include <include/logic/ReportGenerator.h>
// #include <include/ui/ConsoleUI.h>




int callback(
    void *user_data,   // То же, что передано в sqlite3_exec
    int num_columns,   // Количество колонок в строке
    char **columns,    // Массив значений колонок (строки в UTF-8)
    char **col_names   // Массив названий колонок
) {
    std::cout << "Now works callback function\n";
    return 0;
}



int main() {
    // 1. Создаём базу данных (ядро системы)
    // Database db("data/pos.db");

    // // 2. Инициализируем DAO
    // ProductDAO productDAO(db);
    // SaleDAO saleDAO(db);
    // CustomerDAO customerDAO(db);
    // EmployeeDAO employeeDAO(db);

    // // 3. Создаём сервисы
    // ProductService productService(productDAO);
    // SaleService saleService(saleDAO, productService);
    // CustomerService customerService(customerDAO);
    // EmployeeService employeeService(employeeDAO);


    // // 4. Запускаем UI
    // ConsoleUI ui(productService, saleService, customerService, employeeService);
    // ui.run();

    sqlite3* bd;
    char* error_msg = nullptr;

    if (sqlite3_open("../../SQLite_test_project/new_bd.sqlite", &bd) == SQLITE_OK) {
        std::cout << "Database opened\n";
    } else {
        std::cout << "Error: " << sqlite3_errmsg(bd) << '\n';
    }

    // int result = sqlite3_exec(bd, "INSERT INTO [Subject] ([name]) VALUES ('Basic Python')", nullptr, nullptr, &error_msg);

    // if (result == SQLITE_OK) {
    //     std::cout << "Good request\n";
    // } else {
    //     std::cout << "Request error: " << error_msg << '\n';
    //     sqlite3_free(error_msg);
    // }


    int result = sqlite3_exec(bd, "DELETE FROM [Subject] WHERE [name] = 'Basic Python'", callback, nullptr, &error_msg);

    if (result == SQLITE_OK) {
        std::cout << "Good request\n";
    } else {
        std::cout << "Request error: " << error_msg << '\n';
        sqlite3_free(error_msg);
    }

    sqlite3_close(bd);
    return 0;
}