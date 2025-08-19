#include "include/ui/ConsoleUI.h"


void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void ConsoleUI::run() {
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: 
            handleCustomer(); 
            break;
        case 2: 
            handleEmployee(); 
            break;
        case 3: 
            handleProduct(); 
            break;
        case 4:
            handleSale();
            break;
        case 5:
            ReportGenerator();
            break;
        case 0: 
            std::cout << "Exit...\n"; 
            break;
        default: 
            std::cout << "Incorrect input!\n";
        }
    } while (choice != 0);
}


void showMainMenu() {
    std::cout << "\n==== POS System ====\n";
    std::cout << "1. Product management\n";
    std::cout << "2. Sales\n";
    std::cout << "3. Customers\n";
    std::cout << "4. Employees\n";
    std::cout << "5. Reports\n";
    std::cout << "0. Exit\n";
    std::cout << "Select an action: ";
}







//______________________________________________________________________________
//_______________________________Customers______________________________________
//______________________________________________________________________________

void console_registerCustomer();
void console_updateCustomer();
void console_deleteCustomer();
void console_getAllCustomers();
void console_getCustomerById();
void console_getCustomerPurchases();
void console_findCustomerByEmailOrPhone();

void handleCustomer() {
    int choice;
    do {
        showCustomersMenu();
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: 
            console_registerCustomer(); 
            break;
        case 2: 
            console_updateCustomer(); 
            break;
        case 3: 
            console_deleteCustomer(); 
            break;
        case 4:
            console_getAllCustomers();
            break;
        case 5:
            console_getCustomerById();
            break;
        case 6:
            console_getCustomerPurchases();
            break;
        case 7:
            console_findCustomerByEmailOrPhone();
            break;
        case 0: 
            std::cout << "Exit...\n"; 
            break;
        default: 
            std::cout << "Incorrect input!\n";
        }
    } while (choice != 0);
}

void showCustomersMenu() {
    std::cout << "\n==== Customers_Servise menu ====\n";
    std::cout << "1. Registering a new client\n";
    std::cout << "2. Updating client data\n";
    std::cout << "3. Removing a client from the database\n";
    std::cout << "4. List of all clients\n";
    std::cout << "5. Customer information\n";
    std::cout << "6. Customer's purchase history\n";
    std::cout << "7. Customer search by email, phone or additional contact\n";
    std::cout << "0. Exit\n";
    std::cout << "Select an action: ";
}


void console_registerCustomer() {
    std::string name, email, phone;
    std::cout << "The obligatory part of registration\n";
    std::cout << "Enter the client's name: ";
    std::getline(std::cin, name);
    std::cout << "Enter the client's phone number: ";
    std::cin >> phone;
    std::cout << "Enter the client's email: ";
    std::cin >> email;
    std::cout << "Additional information\n"
                 "If you do not have such data, then enter '-'\n";
    std::optional<std::string> address, birth_date, additional_contact;
    std::string tmp_data;
    std::cout << "Enter the client's address: ";
    std::cin >> tmp_data;
    if (tmp_data != "-") {
        address.emplace(tmp_data);
    }
    std::cout << "Enter the client's birth_date (DD.MM.YYYY): ";
    std::cin >> tmp_data;
    if (tmp_data != "-") {
        birth_date.emplace(tmp_data);
    }
    std::cout << "Enter the client's additional contact: ";
    if (tmp_data != "-") {
        additional_contact.emplace(tmp_data);
    }
    if (registerCustomer(name, email, phone, address, birth_date, additional_contact)) {
        std::cout << "New client successfully added\n";
    } else {
        std::cout << "Error: an error occurred when trying to add a new client\n";
    }
}


void console_updateCustomer() {
    size_t customer_id;
    std::cout << "Enter the ID of the client whose information you want to update: ";
    std::cin >> customer_id;

    std::optional<std::string> name, email, phone, address, birth_date, additional_contact;
    std::string tmp_data;
    std::cout << "Enter new information about the customer\n"
                 "If you do not plan to update the information in the requested field, enter '-'\n";
    std::cout << "Enter a new client's name: ";
    std::getline(std::cin, tmp_data);
    if (tmp_data != "-") {
        name.emplace(tmp_data);
    }
    std::cout << "Enter a new client's phone number: ";
    std::cin >> tmp_data;
    if (tmp_data != "-") {
        phone.emplace(tmp_data);
    }
    std::cout << "Enter a new client's email: ";
    std::cin >> tmp_data;
    if (tmp_data != "-") {
        email.emplace(tmp_data);
    }
    std::cout << "Enter a new client's address: ";
    std::cin >> tmp_data;
    if (tmp_data != "-") {
        address.emplace(tmp_data);
    }
    std::cout << "Enter the client's birth_date (DD.MM.YYYY): ";
    std::cin >> tmp_data;
    if (tmp_data != "-") {
        birth_date.emplace(tmp_data);
    }
    std::cout << "Enter the client's additional contact: ";
    if (tmp_data != "-") {
        additional_contact.emplace(tmp_data);
    }

    if (updateCustomer(customer_id, name, email, phone, address, birth_date, additional_contact)) {
        std::cout << "The client's information has been successfully changed\n";
    } else {
        std::cout << "Error: an error occurred while trying to update the client information\n";
    }
}


void console_deleteCustomer() {
    size_t customer_id;
    std::cout << "enter the ID of the client you want to delete from the database: ";
    std::cin >> customer_id;

    if (deleteCustomer(customer_id)) {
        std::cout << "The client has been deleted from the database\n";
    } else {
        std::cout << "Error: an error occurred while trying to deleted the client\n"
    }
}


void console_getAllCustomers() {
    std::vector<Customer> customers = std::move(getAllCustomers());
    const int id_width = 5;
    const int name_width = 20;
    const int email_width = 25;
    const int phone_width = 15;
    const int address_width = 25;
    const int loyalty_width = 15;
    const int birth_width = 12;
    const int contact_width = 20;
    

    std::cout << std::left 
              << std::setw(id_width) << "ID" << " "
              << std::setw(name_width) << "Name" << " "
              << std::setw(email_width) << "Email" << " "
              << std::setw(phone_width) << "Phone" << " "
              << std::setw(address_width) << "Address" << " "
              << std::setw(loyalty_width) << "Loyalty Status" << " "
              << std::setw(birth_width) << "Birth Date" << " "
              << std::setw(contact_width) << "Additional Contact" << "\n";

    std::cout << std::setfill('-') 
              << std::setw(id_width) << "" << " "
              << std::setw(name_width) << "" << " "
              << std::setw(email_width) << "" << " "
              << std::setw(phone_width) << "" << " "
              << std::setw(address_width) << "" << " "
              << std::setw(loyalty_width) << "" << " "
              << std::setw(birth_width) << "" << " "
              << std::setw(contact_width) << "" << "\n"
              << std::setfill(' ');

    for (const auto& customer : customers) {
        std::cout << std::left 
                  << std::setw(id_width) << customer.id << " "
                  << std::setw(name_width) << customer.name << " "
                  << std::setw(email_width) << customer.email << " "
                  << std::setw(phone_width) << customer.phone << " "
                  << std::setw(address_width) << customer.address << " "
                  << std::setw(loyalty_width) << customer.loyalty_status << " "
                  << std::setw(birth_width) << customer.birth_date << " "
                  << std::setw(contact_width) << customer.additional_contact << "\n";
    }

    std::cout << "----------------------------------------------------------------------\n";
}


void console_getCustomerById() {
    size_t customer_id;
    std::cout << "Enter the ID of the client whose information you want to get: ";
    std::cin >> customer_id;

    std::optional<Customer> customer = std::move(getCustomerById(customer_id));

    if (customer != std::nullopt) {
        std::cout << "Information:\n"
        std::cout << "ID: " << customer.id << '\n'
                << "Name: " << customer.name << '\n'
                << "Email: " << customer.email << '\n'
                << "Phone: " << customer.phone << '\n'
                << "Address: " << customer.address << '\n'
                << "Loyalty Status: " << customer.loyalty_status << '\n'
                << "Birth Date: " << customer.birth_date << '\n'
                << "Additional Contact: " << customer.additional_contact << '\n';
    } else {
        std::cout << "Error: information about the client with the entered id was not found\n";
    }
}


void console_getCustomerPurchases() {
    size_t customer_id;
    std::cout << "Enter the ID of the customer whose purchase history you would like to see: ";
    std::cin >> customer_id;

    std::vector<Sale> sales = std::move(getCustomerPurchases(customer_id));

    if (!sales.empty()) {
        const int id_width = 5;
        const int employee_width = 12;
        const int total_width = 10;
        const int date_width = 12;

        std::cout << std::left 
                  << std::setw(id_width) << "ID" << " "
                  << std::setw(employee_width) << "Employee ID" << " "
                  << std::setw(total_width) << "Total" << " "
                  << std::setw(date_width) << "Date" << "\n";
        
        std::cout << std::setfill('-') 
                  << std::setw(id_width) << "" << " "
                  << std::setw(employee_width) << "" << " "
                  << std::setw(total_width) << "" << " "
                  << std::setw(date_width) << "" << "\n"
                  << std::setfill(' ');

        for (const auto& sale : sales) {
            std::cout << std::left 
                      << std::setw(id_width) << sale.id << " "
                      << std::setw(employee_width) << sale.employee_id << " "
                      << std::setw(total_width) << sale.total << " "
                      << std::setw(date_width) << sale.date << "\n";
        }
    } else {
        std::cout << "You entered an incorrect ID, or this customer has not made any purchases yet\n";
    }

}


void console_findCustomerByEmailOrPhone() {
    std::cout << "Enter the client's name, email, or username to search for it\n";
    std::string data;
    std::cin >> data;

    std::vector<Customer> customers = std::move(findCustomerByEmailOrPhone(data));

    if (!customers.empty()) {
        std::cout << "Founded customers:\n"

        for (const auto& customer : customers) {
            std::cout << "ID: " << customer.id << '\n'
                      << "Name: " << customer.name << '\n'
                      << "Email: " << customer.email << '\n'
                      << "Phone: " << customer.phone << '\n'
                      << "Address: " << customer.address << '\n'
                      << "Loyalty Status: " << customer.loyalty_status << '\n'
                      << "Birth Date: " << customer.birth_date << '\n'
                      << "Additional Contact: " << customer.additional_contact << '\n';
            if (customers.size() != 1) {
                std::cout << "-------------------------------------------------\n";
            }
        }
    } else {
        std::cout << "Couldn't find such a client.\n";
    }
}