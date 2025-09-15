#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class Employee {
  private:
    std::string id;
    std::string name;
    std::string position;
    std::string email;
    std::string phone;

  public:
    // constructor
    Employee(const std::string &id, const std::string &name,
             const std::string &position, const std::string &email,
             const std::string &phone)
        : id(id), name(name), position(position), email(email), phone(phone) {}

    // getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPosition() const { return position; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }

    // setters
    void setId(const std::string &newId) { id = newId; }
    void setName(const std::string &newName) { name = newName; }
    void setPosition(const std::string &newPosition) { position = newPosition; }
    void setEmail(const std::string &newEmail) { email = newEmail; }
    void setPhone(const std::string &newPhone) { phone = newPhone; }

    static bool isValidId(const std::string &id) {
        if (id.length() != 3 || !std::all_of(id.begin(), id.end(), ::isdigit)) {
            return false;
        }
        return true;
    }

    static bool isValidEmail(const std::string &email) {
        const std::regex pattern(
            "^[_a-z0-9-]+(\\.[_a-z0-9-]+)*@(?:\\w+\\.)+\\w+$");
        return std::regex_match(email, pattern);
    }

    static bool isValidPhone(const std::string &phone) {
        const std::regex pattern("^[0-9]{2,3}-[0-9]{3,4}-[0-9]{4}$");
        return std::regex_match(phone, pattern);
    }

    void print() const {
        std::cout << id << " " << name << " " << position << " " << email << " "
                  << phone << "\n";
    }
};

class EmployeeManager {
  private:
    std::vector<Employee> employees;
    const std::string filename = "data.txt";
    const char delimiter = ',';

  public:
    EmployeeManager() {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Coudl not open the file: " << filename << "\n";
            return;
        }

        std::string line;
        while (getline(inFile, line)) {
            std::vector<std::string> fields;
            size_t delimPos = 0;
            size_t prev = 0;

            while ((delimPos = line.find(delimiter, prev)) !=
                   std::string::npos) {
                fields.push_back(line.substr(prev, delimPos - prev));
                prev = delimPos + 1;
            }

            fields.push_back(line.substr(prev));
            if (fields.size() == 5) {
                employees.emplace_back(fields[0], fields[1], fields[2],
                                       fields[3], fields[4]);
            }
        }

        inFile.close();
    }

    ~EmployeeManager() {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Coudl not open the file: " << filename << "\n";
            return;
        }

        for (Employee &emp : employees) {
            outFile << emp.getId() << delimiter << emp.getName() << delimiter
                    << emp.getPosition() << delimiter << emp.getEmail()
                    << delimiter << emp.getPhone() << "\n";
        }

        outFile.close();
    }

    void addEmployee() {
        std::string id;
        std::string name;
        std::string position;
        std::string email;
        std::string phone;

        std::cout << "Insert new employee\n";
        do {
            std::cout << "Insert id (3 digits number, e.g 102): ";
            std::getline(std::cin, id);
            if (!isValidId(id)) {
                std::cerr << "\nError: invalid id: " << id << "\n";
            }
        } while (!isValidId(id));

        std::cout << "\nInsert name: ";
        getline(std::cin, name);
        std::cout << "\nInsert position: ";
        getline(std::cin, position);

        do {
            std::cout << "\nInsert email: ";
            std::getline(std::cin, email);
            if (!Employee::isValidEmail(email)) {
                std::cerr << "\nError: invalid email: " << email << "\n";
            }
        } while (!Employee::isValidEmail(email));

        do {
            std::cout << "\nInsert phone: ";
            std::getline(std::cin, phone);
            if (!Employee::isValidPhone(phone)) {
                std::cerr << "\nError: invalid phone: " << phone << "\n";
            }
        } while (!Employee::isValidPhone(phone));

        employees.emplace_back(id, name, position, email, phone);
        std::cout << "Employee inserted sucessfully.\n";
    }

    void listEmployees() {
        if (employees.empty()) {
            std::cout << "No employees found.\n";
            return;
        }

        for (Employee &emp : employees) {
            emp.print();
        }
    }

    void viewEmployee() {
        if (employees.empty()) {
            std::cout << "No employees found.\n";
            return;
        }

        std::string id;
        do {
            std::cout << "Insert Id: ";
            getline(std::cin, id);
            Employee *emp = findId(id);
            if (emp != nullptr) {
                emp->print();
                return;
            }

            std::cerr << "Employee not found!\n";
        } while (true);
    }

    void editEmployee() {
        if (employees.empty()) {
            std::cout << "No employees found.\n";
            return;
        }

        std::string id;
        std::string name;
        std::string position;
        std::string email;
        std::string phone;

        Employee *emp = nullptr;
        do {
            std::cout << "Insert Id: ";
            getline(std::cin, id);
            emp = findId(id);
            if (emp != nullptr) {
                break;
            }

            std::cerr << "\nEmployee not found!\n";
        } while (true);

        std::cout << "\nUpdate name: ";
        getline(std::cin, name);
        emp->setName(name);

        std::cout << "\nUpdate position: ";
        getline(std::cin, position);
        emp->setPosition(position);

        do {
            std::cout << "\nUpdate email: ";
            getline(std::cin, email);
            if (!Employee::isValidEmail(email)) {
                std::cerr << "\nError invalid email: " << email << "\n";
            }
        } while (!Employee::isValidEmail(email));
        emp->setEmail(email);

        do {
            std::cout << "\nUpdate phone: ";
            getline(std::cin, phone);
            if (!Employee::isValidPhone(phone)) {
                std::cerr << "\nError invalid phone: " << phone << "\n";
            }
        } while (!Employee::isValidPhone(phone));

        emp->setPhone(phone);
        std::cout << "Successfully updated employee.\n";
    }

    void deleteEmployee() {
        if (employees.empty()) {
            std::cout << "No employees found.\n";
            return;
        }

        std::string id;
        do {
            std::cout << "\nInsert id: ";
            getline(std::cin, id);

            auto it = std::find_if(
                employees.begin(), employees.end(),
                [&](const Employee &emp) { return emp.getId() == id; });

            if (it != employees.end()) {
                employees.erase(it);
                std::cout << "\nEmployee deleted successfully.\n";
                return;
            }
        } while (true);
    }

    Employee *findId(const std::string &id) {
        for (Employee &emp : employees) {
            if (emp.getId() == id) {
                return &emp;
            }
        }

        return nullptr;
    }

    bool isValidId(const std::string &id) {
        if (!findId(id) && !Employee::isValidId(id)) {
            return false;
        }

        return true;
    }
};

void displayMenu() {
    std::cout << "Employee Manager Menu:\n";
    std::cout << "1. Add an Employee\n";
    std::cout << "2. List All Employees\n";
    std::cout << "3. View an Employee\n";
    std::cout << "4. Edit an Employee\n";
    std::cout << "5. Delete an Employee\n";
    std::cout << "6. Exit\n";
}

int main() {
    int choice;
    EmployeeManager manager;
    do {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            manager.addEmployee();
            break;
        case 2:
            manager.listEmployees();
            break;
        case 3:
            manager.viewEmployee();
            break;
        case 4:
            manager.editEmployee();
            break;
        case 5:
            manager.deleteEmployee();
            break;
        case 6:
            break;
        default:
            std::cout << "\nInvalid choice!\n";
            break;
        }
    } while (choice != 6);
    return 0;
}
