# POS-система (Point of Sale)

***POS-система (Point of Sale)*** — это программа, которая помогает магазину или кафе вести учёт товаров, клиентов и продаж.

Функционал:
1. ***Управление товарами***:
    - Добавление / редактирование / удаление товара
    - Отображение списка товаров
    - Поиск по названию
    - Учет остатков (stock уменьшается при продаже)

2. ***Продажа***:
    - Ввод списка продуктов с указанием количества
    - Расчёт:
        * Сумма без налогов
        * Скидки на уровне товара и/или чека
        * Налог на уровне товара
    - Сохранение чека в таблицы `sales` и `sale_items`
    - Вывод чека в консоль или файл

3. ***Клиенты***
    - Регистрация клиента
    - Привязка клиента к чеку (по желанию)
    - Возможность анализа покупок по клиенту

4. ***Сотрудники***
    - Добавление сотрудников
    - Простая авторизация
    - Логирование, кто провел продажу

5. ***Отчёты***
    - Ежедневные/ежемесячные продажи
    - Продажи по сотруднику
    - Продажи по клиенту
    - Остатки на складе
    - Самые продаваемые товары


### Предполагаемая структура проекта

/pos-system/
├── CMakeLists.txt
├── main.cpp
├── src/
│   ├── db/
│   │   ├── Database.cpp
│   │   ├── ProductDAO.cpp
│   │   └── ...
│   ├── logic/
│   │   ├── SaleService.cpp
│   │   └── ...
│   └── ui/
│       ├── ConsoleUI.cpp
│       └── ...
├── include/
│   └── ...
└── data/
    └── pos.db


**CMakeLists.txt**
- Скрипт для сборки проекта через CMake
- Описывает, какие исходники компилировать, какие папки с заголовками подключать, где лежит SQLite


**main.cpp**
- Точка входа программы (int main()):
    * Инициализация базы данных
    * Создание основных объектов (UI, логика)
    * Запуск главного цикла программы

___

**src/** — исходный код; делится на подмодули по логике.
___

**src/db/** — работа с базой данных (DAO слой)
*DAO* — Data Access Object, слой, который общается с SQLite напрямую.
- Database.cpp
    * Отвечает за подключение к SQLite
    * Открывает/закрывает базу, выполняет SQL-запросы
    * Может содержать метод executeQuery(), executeUpdate() и т.п.
- ProductDAO.cpp
    * Класс для работы только с таблицей products
    * Методы типа getAllProducts(), addProduct(), updateProductStock()
    * Получает/отправляет данные в Database

(и т.д. для CustomerDAO, SaleDAO)
 ___

**src/logic/** — бизнес-логика
Здесь код, который реализует правила работы POS-системы.
- SaleService.cpp
    - Регистрирует продажи
    - Считает скидки и налоги
    - Обновляет остатки на складе
    - Записывает результат в базу через DAO
(и т.д. для ProductService, CustomerService, EmployeeService)

Можно добавить ReportGenerator.cpp для отчётов

___

**src/ui/** — интерфейс пользователя
Отвечает за ввод/вывод данных.
- ConsoleUI.cpp
    - Консольное меню (текстовый интерфейс)
    - Спрашивает пользователя, что он хочет сделать
    - Передаёт команды в бизнес-логику (SaleProcessor, ProductDAO и др.)
    - Показывает результаты (списки товаров, отчёты)

Можно добавить QtUI.cpp или что-то другое

___

**include/** — заголовочные файлы
Хранят .h (или .hpp) файлы для всех .cpp из src/
Пример: include/db/Database.h, include/logic/SaleProcessor.h
Нужны, чтобы отделить интерфейс класса от его реализации

___

**data/** — данные программы
pos.db — сама база данных SQLite
Тут же можно хранить тестовые дампы (init.sql), чтобы быстро заполнить базу стартовыми данными




### Схема взаимодействия

![alt text](image.png)





### Папки logic

Папки `\src\logic\` и `\include\logic\` - хранят в себе файлы реализации и заголовочные файлы следующих классов: `ProductService`, `CustomerService`, `EmployeeService`, `SaleService`, - они призваны инкапсулировать в себе все всю бизнесс логику, то есть мы хотим создать прослойки между базой данных и интерфейсом, которые будет содержать в себе все функции, необходимые пользователю. Поговорим о каждом из перечисленных классов отдельно и определим, какими методами он должен обладать:

**1. ProductService**
Отвечает за управление товарами и их остатками.
Методы:
- `addProduct(name, price, stock, tax_rate, discount)` — добавление нового товара.
- `updateProduct(id, name, price, stock, tax_rate, discount)` — обновление данных товара.
- `deleteProduct(id)` — удаление товара.
- `getAllProducts()` — возврат списка всех товаров.
- `searchProductsByName(name)` — поиск товаров по названию.
- `updateStock(product_id, quantity)` — обновление остатков (уменьшение при продаже или ручное изменение).
- `getProductById(id)` — получение информации о конкретном товаре.

Возможные дополнения:
- Можно добавить методы для применения скидок/налогов к товарам, например `applyDiscount(product_id, discount)`.
- Для отчётов: `getLowStockProducts(threshold)` — возвращает товары с остатком ниже указанного порога.

**2. CustomerService**
Управление клиентами и их покупками.
Методы:
- `registerCustomer(name, email, phone, address, birth_date, additional_contact)` — регистрация нового клиента.
- `updateCustomer(id, name, email, phone, address, birth_date, additional_contact)` — обновление данных клиента.
- `deleteCustomer(id)` — удаление клиента из базы данных
- `getAllCustomers()` — список всех клиентов.
- `getCustomerById(id)` — информация о клиенте.
- `getCustomerPurchases(id)` — возврат истории покупок клиента.
- `findCustomerByEmailOrPhone(query)` — поиск клиента по email или телефону.

Возможные дополнения:
- `getTopCustomersBySpending(limit)` — клиенты с наибольшими суммами покупок.
- `getCustomersByPurchaseCount(min_orders)` — фильтрация по количеству заказов.

**3. EmployeeService**
Работа с сотрудниками и их авторизацией.
Методы:
- `addEmployee(name, role, login, password)` — добавление сотрудника.
- `updateEmployee(id, name, role)` — обновление данных (без логина/пароля для безопасности).
- `deleteEmployee(id)` — удаление сотрудника.
- `authenticate(login, password)` — проверка учетных данных (возвращает Employee или nullptr).
- `getAllEmployees()` — список всех сотрудников.
- `getEmployeeById(id)` — информация о сотруднике.
- `changePassword(id, new_password)` — смена пароля.

Возможные дополнения:
- Для отчётов: `getSalesByEmployee(id, start_date, end_date)` — продажи сотрудника за период.
- `getEmployeesByRole(role)` — фильтрация по должности.


**4. SaleService**
Обработка продаж, расчеты и формирование чеков.
Методы:
- `createSale(employee_id, customer_id = nullptr)` — создание новой продажи (возвращает sale_id).
- `addProductToSale(sale_id, product_id, quantity)` — добавление товара в чек с расчетом цены, скидки и налога.
- `applySaleDiscount(sale_id, discount)` — скидка на весь чек.
- `finalizeSale(sale_id)` — завершение продажи:
- Расчет итоговой суммы (с учетом налогов и скидок).
- Обновление остатков товаров через `ProductService`.
- Сохранение чека в `sales` и `sale_items`.
- `getSaleDetails(sale_id)` — информация о конкретном чеке.
- `printReceipt(sale_id, output_type)` — вывод чека в консоль/файл (формат: текст или PDF).

Возможные дополнения:
- Для аналитики: `getSalesReport(start_date, end_date)` — отчет по продажам за период.
- `getTopSellingProducts(limit)` — самые популярные товары.


**5. ReportGenerator**
Генерация сложных отчетов.
Методы:
- `generateDailySalesReport(date)` — дневной отчет.
- `generateMonthlySalesReport(month, year)` — месячный отчет.
- `generateInventoryReport()` — остатки на складе.
- `exportReportToFile(report_data, format)` — экспорт в CSV/PDF





### Папки db
Папки `\src\db\` и `\include\db\` - хранят в себе файлы реализации и заголовочные файлы следующих классов: `Database`, `CustomerDAO`, `ProductDAO`, `SaleDAO`, `EmployeeDAO` - эти компоненты отвечают исключительно за работу с базой данных, но не содержат бизнес-логики. Их задача — выполнять CRUD-операции (Create, Read, Update, Delete) и предоставлять данные в удобном формате для сервисов.
Поговорим о каждом из перечисленных классов отдельно и определим, какими методами он должен обладать:

**1. Класс Database**
Обёртка над подключением к SQLite, управление транзакциями и выполнение запросов. `DAO` используют класс `Database` как прослойку:
- `Database` предоставляет высокоуровневые методы (`executeQuery`, `executeUpdate`), которые инкапсулируют работу с SQLite API.
- `DAO` вызывают эти методы, передавая SQL и параметры.

Такой подход выгоден сразу по нескольким причинам:
- Упрощение `DAO`: меньше кода, нет дублирования.
- Централизованное управление ошибками и транзакциями.
- Легче подменить БД (например, на PostgreSQL) — меняется только Database.

Функционал:
- Подключение/отключение
- Выполнение запросов
- Транзакции

**2. CustomerDAO, ProductDAO, SaleDAO, EmployeeDAO**
Нет смысла рассказывать про каждый из этих классов по отдельности, каждый из них предоставляет информацию, запрашиваемую соответствующими `Service` в удобном формате






### Папки ui
Папки `\src\ui\` и `\include\ui\` - хранят в себе файл реализации и заголовочный файл класса: `ConsoleUI` - он содержит интерфейс приложения.
Поскольку с точки зрения архитектуры было бы правильно вынести реализацию из `main` в отдельный файл, то необходим класс `ConsoleUI`, который будет инкапсулировать в себе весь функционал взаимодейсвия с пользователем. Более того, одним из запросов при дальнейшем развитии проекта может быть создание оконного приложения, что потребует полной переработки интерфейса, то есть его компоненты должны быть аккуратно выделены в отдельные файлы.

### Примерная структура таблиц 

```sql
-- Таблица сотрудников
CREATE TABLE employees (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    role TEXT,
    login TEXT UNIQUE,
    password TEXT
);

-- Таблица клиентов
CREATE TABLE customers (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    email TEXT,
    phone TEXT,
    address TEXT,
    loyalty_status TEXT DEFAULT 'regular',
    birth_date TEXT,
    additional_contact TEXT
);

-- Таблица продуктов
CREATE TABLE products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    price REAL NOT NULL,
    stock INTEGER NOT NULL,
    tax_rate REAL DEFAULT 0.0,
    discount REAL DEFAULT 0.0
);

-- Таблица продаж (чеков)
CREATE TABLE sales (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER,
    employee_id INTEGER,
    total REAL,
    date TEXT,
    FOREIGN KEY(customer_id) REFERENCES customers(id),
    FOREIGN KEY(employee_id) REFERENCES employees(id)
);

-- Таблица позиций в продаже (проданные товары)
CREATE TABLE sale_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sale_id INTEGER,
    product_id INTEGER,
    quantity INTEGER,
    unit_price REAL,
    discount REAL,
    tax REAL,
    FOREIGN KEY(sale_id) REFERENCES sales(id),
    FOREIGN KEY(product_id) REFERENCES products(id)
);
```


## Реализация

### Интерфейс - ui

Итак, как и было сказано ранее компоненты интерфейся располагаются в папках `\src\ui` и `\include\ui`, там располгаются файлы `ConsoleUI.cpp` и `ConsoleUI.h` соответственно. Поскольку пока что я хочу реализовать консольное приложение, то взаимодействие пользователя с программой будет осуществляться через консоль.
Пользователю должны быть доступны все методы, которые будут реалзованы в `...Servis`, то есть он должен иметь возможность просмотреть доступные функции и выбрать необходимые. Таким образом мы приходим к нехитрой мысли - класс `ConsoleUI` должен иметь доступ к объектам классов `...Servis`, но тут же возникает другой вопрос - а должен ли интерфейс сам создавать сервисы или же иметь к ним доступ только по ссылке? Немного подумав, я решил, что `ConsoleUI` должен отвечать только за взаимодействие с пользователем - ввод/вывод данных. То есть сервисы будут создаваться в `main`, а `ConsoleUI` будет хранить ссылки на них.
Реализуемый класс будет обладать единственным методом - `run()`, который с свою очередь будет выводить в консоль главное меню:

```c++
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
```
И взависимости от выбора пользователя вызывать функции: 
- `handleCustomer()` - управление работниками
- `handleEmployee()` - управление персооналом
- `handleProduct()` - управление с реализуемыми продуктами
- `handleSale()` - упраление списком продаж
- `ReportGenerator()` - создание отчётов

Все эти функции будут иметь схожую реализацию поэтому, более подробно мы рассмотри лишь одну из них - `handleCustomer()`. Поведение этой функции в сущности похоже на то, что делает `run()`, то есть выводит меню, где описывается функционал, который доступен пользователю при взаимодействии с таблицей `customers`:

```c++
void showCustomersMenu() {
    std::cout << "\n==== Customers_Servise menu ====\n";
    std::cout << "1. Registering a new client\n";
    std::cout << "2. Updating client data\n";
    std::cout << "3. Removing a client from the database\n";
    std::cout << "4. List of all clients\n";
    std::cout << "5. Customer information\n";
    std::cout << "6. Customer's purchase history\n";
    std::cout << "7. Customer search by email or phone\n";
    std::cout << "0. Exit\n";
    std::cout << "Select an action: ";
}
```

В зависимости от выбора пользователя `handleCustomer()` вызывает вспомогательные функции: `console_registerCustomer()`, `console_updateCustomer()` и т.д. Они необходимы для ввода доп информации для вызова соответствующих функции из `CustomerService`, а так же для вывода результата, если это необходимо.

Собственно говоря примерно так и будет выглядеть реализация интерфейса.



### Взаимодействие с БД 

#### Класс Database

Итак, основным назначением этого класса является обработка запросов к базе данных, поступающих из DAO, или же инкапсуляция SQL: класс полностью скрывает детали работы с SQLite API, а также предоставляет простой интерфейс для выполнения запросов.

<u>Интерфейс:</u>
- `executeQuery` - для `SELECT` запросов (возвращает вектор строк)
- `executeUpdate` - для `INSERT/UPDATE/DELETE` (возвращает `bool`)
- Поддержка транзакций

Подробней поговорим про реализацию:
**1. Конструктор**

```c++
Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db_)));
    }
}
```

Принимает путь к базе данных (как относительный, так и абсолютный) и совершает попытку её открыть, в случае успеха мы инициализируем поле `db_` - наше подключение, в противном случае кидаем ошибку

**2. Деструктор**

```c++
Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}
```

Закрывает соедиyение если таковое было открыто, при текущей реализации проверка на то, открыто ли соединие - излишняя, посколько если соединение открыть не получится, то конструктор бросит исключение, но я её всё же оставлю.


**3. Оператор присваивания и конструктор копирования**

```c++
Database(const Database&) = delete;
Database& operator=(const Database&) = delete;
```

Как вы могли догадаться - они запрещены, сразу по нескольким причинам:
- Нам важна уникальность подключения к БД
- Проблемы с транзакциями

**4. Метод `executeQuery`**

Метод, для выполнения запросов, содержащих `SELECT`. Рассказ про этот метод я предлагаю начать с его самой что ни на есть наивной реализации:

```c++

using Row = std::unordered_map<std::string, std::string>;
using Rows = std::vector<Row>;

void sqlite_callback(void* data, int num_columns, char** columns, char** col_names) {
    Rows& rows = *static_cast<Rows*>(data);
    Row row;

    for (int i = 0; i < num_columns; ++i) {
        const char* col_name = col_names[i] ? col_names[i] : "NULL";
        const char* col_value = columns[i] ? columns[i] : "NULL";
        row[col_name] = col_value;
    }

    rows.push_back(std::move(row));
    return 0;
}


Rows executeQuery(const std::string& sql) {
    Rows rows;
    char *errmsg = NULL;

    int result = sqlite3_exec(db_, sql.c_str(), sqlite_callback, &rows, &errmsg);

    if (result != SQLITE_OK) {
        std::string error = errmsg ? errmsg : "Unknown error";
        sqlite3_free(errmsg);
        throw std::runtime_error(string("Error: ") + errmsg);
    }
    return rows;
}
```

Прошу не судить слишком строго - это тестовый пример и я его даже не компилировал, его можно было реализовать немного по-другому, но давайте рассмотрим эту реализацию более маштабно и выделим его главную слабость - использование функции `sqlite3_exec()`, и, пожалуй, я возьму на себя ответственность более менее подробно объяснить, почему же в действительности эта функция может стать проблемной. 
Проведём кратрий ликбез по тому, как работает выбранная мною функция, когда `sqlite3_exec()` выполняет запрос, содеджащий `SELECT`, она без предварительных проверок компилирует запрос и выполняет его, тут нужно уточнить, если вы сделали ошибку в синтаксисе, то, конечно, ничего не скомпилируется, а если в запросе присутствует SQL-инъекция, то она не будет выявлена, то есть проверка на их наличие в запросе ложится на вызывающую функцию, что не вполне удобно.
Таким образом мы и приходим к решению проблемы - использование подготовленных запросов. Их применение благоприятно сказывается как на производительности, так и на использование памяти, но главное - они инкапсулируют в себе проверку на наличие SQL-инъекций,посредством функции биндинга (`sqlite3_bind_*`). И если первые два преимущества проявляют себя лишь при многократном вызове запросов с одним шаблоном, то последний будет исполняться всегда.
Поэтому итоговый вариант реализации этой функции будет выглядеть так:

```c++
Rows Database::executeQuery(const std::string& sql, const std::vector<std::string>& params) {
    checkDbNotNull();
    sqlite3_stmt* stmt = nullptr;
    Rows rows;

    // Поготовка запроса
    checkOk(sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr), 
          "Failed to prepare statement");

    try {
        // Привязка данных (заполнение placeholder-ов)
        for (int i = 0; i < params.size(); ++i) {
            checkOk(sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT),
                  "Failed to bind parameter");
        }

        while (true) {
            // Выполнение запроса
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
```

В коде я уже оговорился, за что отвечает каждый его кусок, тут я лишь подробней напишу про обраблтку ошибок и вспомогательные функции:
- Обработка исключений: если функция `sqlite3_prepare_v2` завершится неудачно, то нам достаточно бросить исключение, при неудачной папытке скомпилировать запрос выделенные ресурсы освобождаются автоматически, совсем иначе обстоит ситуация с функциями `sqlite3_bind_*` и `sqlite3_step`, в случае ошибки мо должны самостоятельно освободить выделенные ресурсы с помощью `sqlite3_finalize` и только после этого бросить исключение, именно это я и релизовал
- Функция `checkDbNotNull` - проверяет, открыто ли соединение с базой данных, в случае, если оно закрыто, кидает исключение
- Функция `checkOk` - проверяет успешно ли завершились функции `sqlite3_bind_*` и `sqlite3_prepare_v2`, в случае неудачи кидает исключение
Так же хотел бы обратить отдельно внимание на выбор параметров для `Row` и `Rows`, я хотел было использовать `std::variant`, размышляя над тем, что не слишком оптимально хранить целые и дробные числа в строке, но решил, что игра не стоит свечи) Возможно неленивый читатель предложит более оптимальный вариант хранения результатов запросов, но пока что я остановлюсь на самом простом и понятном варианте.


**4. Метод `executeUpdate`**

Метод, предназначенный для выполнения запростов не возвращающих данных (`INSERT/UPDATE/DELETE`). Я не буду подробно про него рассказывать, поскольку в его основе лежат все те же идеи, что я реализовал в методе `executeQuery`, приведу код с комментариями

```c++
bool Database::executeUpdate(const std::string& sql, const std::vector<std::string>& params) {
    checkDbNotNull();
    sqlite3_stmt* stmt = nullptr;
    
    // Подготовка запроса (компиляция)
    checkOk(sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr),
          "Failed to prepare statement");

    try {
        // Привязка параметров (заполнение plaseholder-ов)
        for (int i = 0; i < params.size(); ++i) {
            checkOk(sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT),
                  "Failed to bind parameter");
        }
    } catch (...) {
        sqlite3_finalize(stmt);
        throw;
    }

    // Исполнение запроса
    int rc = sqlite3_step(stmt);
    // Освобождение выделенных ресурсов
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        return true;
    } else {
        throw std::runtime_error(std::string("Failed to execute update") + ": " + sqlite3_errmsg(db_));
    }
}
```