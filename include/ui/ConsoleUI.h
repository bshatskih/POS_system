#pragma once




class ConsoleUI {
 public:
    ConsoleUI() = default;
    void run();

 private:
    void mainMenu();
    void manageProducts();
    void manageCustomers();
    void manageEmployees();
    void processSale();
    void viewReports();

 
};

