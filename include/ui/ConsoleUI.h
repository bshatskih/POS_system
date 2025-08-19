#pragma once
#include <iostream>
#include <limits>
#include <optional>
#include <vector>
#include <iomanip>
#include <string>
#include <include/logic/CustomerService.h>
#include <include/logic/EmployeeService.h>
#include <include/logic/ProductService.h>
#include <include/logic/ReportGenerator.h>
#include <include/logic/SaleService.h>






class ConsoleUI {
    CustomerService& customerService;
    EmployeeService& employeeService;
    ProductService& productService;
    SaleService& saleService;
    ReportGenerator& reportGenerator; 

 
 public:
    ConsoleUI(CustomerService& customerService_, EmployeeService& employeeService_, ProductService& productService_, 
             ReportGenerator& reportGenerator_, SaleService& saleService_) 
            : customerService(customerService_), employeeService(employeeService_), productService(productService_), 
              reportGenerator(reportGenerator_), saleService(saleService_)  {}

    void run();
};

