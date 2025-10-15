#include "Network.hpp"
#include <iostream>
#include <limits>

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void showMainMenu() {
    std::cout << "\n===== CareerConnect Main Menu =====\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "===================================\n";
    std::cout << "Enter your choice: ";
}

void showUserMenu(const std::string& username) {
    std::cout << "\n--- Welcome, " << username << "! ---\n";
    std::cout << "1. View My Profile\n";
    std::cout << "2. View News Feed\n";
    std::cout << "3. Create a Post\n";
    std::cout << "4. Search for Users\n";
    std::cout << "5. Send Connection Request\n";
    std::cout << "6. View Connection Requests\n";
    std::cout << "7. Accept Connection Request\n";
    std::cout << "8. Logout\n";
    std::cout << "---------------------------\n";
    std::cout << "Enter your choice: ";
}

void handleRegistration(Network& net) {
    std::string type, uname, pwd, name, field1, field2;
    std::cout << "Are you a (1) Student or (2) Professional? ";
    std::cin >> type;
    clearInputBuffer();

    std::cout << "Enter username: ";
    getline(std::cin, uname);
    std::cout << "Enter password: ";
    getline(std::cin, pwd);
    std::cout << "Enter full name: ";
    getline(std::cin, name);

    if (type == "1") {
        std::cout << "Enter university: ";
        getline(std::cin, field1);
        std::cout << "Enter major: ";
        getline(std::cin, field2);
        auto student = std::make_unique<Student>(uname, pwd, name, field1, field2);
        if (net.addUser(std::move(student))) {
            std::cout << "Student registration successful!\n";
        } else {
            std::cout << "Username already exists. Please try another.\n";
        }
    } else if (type == "2") {
        std::cout << "Enter company: ";
        getline(std::cin, field1);
        std::cout << "Enter job title: ";
        getline(std::cin, field2);
        auto prof = std::make_unique<Professional>(uname, pwd, name, field1, field2);
        if (net.addUser(std::move(prof))) {
            std::cout << "Professional registration successful!\n";
        } else {
            std::cout << "Username already exists. Please try another.\n";
        }
    } else {
        std::cout << "Invalid choice. Please try again.\n";
    }
}

void loggedInLoop(User* currentUser, Network& net) {
    int choice = 0;
    while (choice != 8) {
        showUserMenu(currentUser->getUsername());
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                currentUser->displayProfile();
                break;
            case 2:
                net.viewNewsFeed(currentUser->getUsername());
                break;
            case 3: {
                std::string content;
                std::cout << "What's on your mind? ";
                getline(std::cin, content);
                net.createPost(currentUser->getUsername(), content);
                break;
            }
            case 4: {
                std::string query;
                std::cout << "Search by name or username: ";
                getline(std::cin, query);
                net.searchUsers(query);
                break;
            }
            case 5: {
                std::string targetUser;
                std::cout << "Enter username to connect with: ";
                getline(std::cin, targetUser);
                net.sendConnectionRequest(currentUser->getUsername(), targetUser);
                break;
            }
            case 6:
                net.viewConnectionRequests(currentUser->getUsername());
                break;
            case 7: {
                std::string requestUser;
                std::cout << "Enter username of the request to accept: ";
                getline(std::cin, requestUser);
                net.acceptConnectionRequest(currentUser->getUsername(), requestUser);
                break;
            }
            case 8:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

int main() {
    Network net;
    int choice = 0;

    // Pre-populate with some data for a better demo
    net.addUser(std::make_unique<Professional>("jdoe", "pass123", "John Doe", "Innovate Inc.", "Software Engineer"));
    net.addUser(std::make_unique<Student>("asmith", "pass123", "Alice Smith", "State University", "Computer Science"));

    while (choice != 3) {
        showMainMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
            choice = 0; // Reset choice
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                handleRegistration(net);
                break;
            case 2: {
                std::string uname, pwd;
                std::cout << "Enter username: ";
                getline(std::cin, uname);
                std::cout << "Enter password: ";
                getline(std::cin, pwd);

                User* currentUser = net.login(uname, pwd);
                if (currentUser) {
                    std::cout << "Login successful!\n";
                    loggedInLoop(currentUser, net);
                } else {
                    std::cout << "Invalid username or password.\n";
                }
                break;
            }
            case 3:
                std::cout << "Exiting CareerConnect. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}