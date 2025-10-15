#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

class Post {
private:
    std::string authorUsername;
    std::string content;
    int likes;

public:
    Post(const std::string& author, const std::string& text)
        : authorUsername(author), content(text), likes(0) {}

    void display() const {
        std::cout << "    \"" << content << "\"\n";
        std::cout << "    - " << authorUsername << " | Likes: " << likes << std::endl;
    }

    void likePost() {
        likes++;
    }

    std::string getAuthor() const {
        return authorUsername;
    }
};

// Base class for all users - demonstrates Abstraction
class User {
protected:
    std::string username;
    std::string password;
    std::string fullName;
    std::vector<std::string> connections; // Store usernames of connections

public:
    User(const std::string& uname, const std::string& pwd, const std::string& name)
        : username(uname), password(pwd), fullName(name) {}

    // Virtual destructor for base class
    virtual ~User() = default;

    // Pure virtual function - makes User an abstract class and enforces polymorphism
    virtual void displayProfile() const = 0;

    std::string getUsername() const { return username; } //:: scope resolution operator
    std::string getFullName() const { return fullName; }

    bool checkPassword(const std::string& pwd) const {
        return password == pwd;
    }

    void addConnection(const std::string& connUsername) {
        connections.push_back(connUsername);
    }

    const std::vector<std::string>& getConnections() const {
        return connections;
    }

    bool isConnectedTo(const std::string& otherUsername) const {
        return std::find(connections.begin(), connections.end(), otherUsername) != connections.end();
    }
};

// Derived class for Students
class Student : public User {
private:
    std::string university;
    std::string major;

public:
    Student(const std::string& uname, const std::string& pwd, const std::string& name, const std::string& uni, const std::string& maj)
        : User(uname, pwd, name), university(uni), major(maj) {}

    void displayProfile() const override {
        std::cout << "\n--- Student Profile ---\n";
        std::cout << "Name: " << fullName << " (@" << username << ")\n";
        std::cout << "University: " << university << "\n";
        std::cout << "Major: " << major << "\n";
        std::cout << "Connections: " << connections.size() << "\n";
        std::cout << "-----------------------\n";
    }
};

// Derived class for Professionals
class Professional : public User {
private:
    std::string company;
    std::string jobTitle;

public:
    Professional(const std::string& uname, const std::string& pwd, const std::string& name, const std::string& comp, const std::string& title)
        : User(uname, pwd, name), company(comp), jobTitle(title) {}

    void displayProfile() const override {
        std::cout << "\n--- Professional Profile ---\n";
        std::cout << "Name: " << fullName << " (@" << username << ")\n";
        std::cout << "Company: " << company << "\n";
        std::cout << "Title: " << jobTitle << "\n";
        std::cout << "Connections: " << connections.size() << "\n";
        std::cout << "--------------------------\n";
    }
};

#endif // USER_HPP