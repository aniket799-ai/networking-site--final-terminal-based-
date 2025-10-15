#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "User.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>

class Network {
private:
    // Using a map for efficient user lookup by username.
    // Using unique_ptr for automatic memory management of User objects.
    std::map<std::string, std::unique_ptr<User>> users;
    std::vector<Post> posts;
    std::map<std::string, std::vector<std::string>> connectionRequests; // Key: recipient, Value: list of senders

public:
    Network() = default;

    User* findUser(const std::string& username) {
        auto it = users.find(username);
        if (it != users.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool addUser(std::unique_ptr<User> newUser) {
        if (!newUser || findUser(newUser->getUsername())) {
            return false; // User already exists or is null
        }
        users[newUser->getUsername()] = std::move(newUser);
        return true;
    }

    User* login(const std::string& username, const std::string& password) {
        User* user = findUser(username);
        if (user && user->checkPassword(password)) {
            return user;
        }
        return nullptr;
    }

    void sendConnectionRequest(const std::string& fromUser, const std::string& toUser) {
        if (findUser(toUser) && fromUser != toUser) {
            // Avoid duplicate requests
            auto& requests = connectionRequests[toUser];
            if (std::find(requests.begin(), requests.end(), fromUser) == requests.end()) {
                requests.push_back(fromUser);
                std::cout << "Connection request sent to " << toUser << ".\n";
            } else {
                std::cout << "You have already sent a request to " << toUser << ".\n";
            }
        } else {
            std::cout << "User not found or you cannot connect with yourself.\n";
        }
    }

    void viewConnectionRequests(const std::string& username) {
        if (connectionRequests.find(username) == connectionRequests.end() || connectionRequests[username].empty()) {
            std::cout << "You have no pending connection requests.\n";
            return;
        }

        std::cout << "\n--- Pending Connection Requests ---\n";
        for (const auto& sender : connectionRequests[username]) {
            std::cout << "- " << sender << std::endl;
        }
    }

    void acceptConnectionRequest(const std::string& currentUser, const std::string& requestUser) {
        auto& requests = connectionRequests[currentUser];
        auto it = std::find(requests.begin(), requests.end(), requestUser);

        if (it != requests.end()) {
            User* user1 = findUser(currentUser);
            User* user2 = findUser(requestUser);
            if (user1 && user2) {
                user1->addConnection(requestUser);
                user2->addConnection(currentUser);
                requests.erase(it); // Remove the request
                std::cout << "You are now connected with " << requestUser << ".\n";
            }
        } else {
            std::cout << "No connection request found from " << requestUser << ".\n";
        }
    }

    void createPost(const std::string& author, const std::string& content) {
        posts.emplace_back(author, content);
        std::cout << "Post created successfully!\n";
    }

    void viewNewsFeed(const std::string& username) {
        User* user = findUser(username);
        if (!user) return;

        std::cout << "\n--- Your News Feed ---\n";
        bool foundPosts = false;
        for (const auto& post : posts) {
            // Show posts from self or connections
            if (post.getAuthor() == username || user->isConnectedTo(post.getAuthor())) {
                post.display();
                std::cout << "------------------------\n";
                foundPosts = true;
            }
        }

        if (!foundPosts) {
            std::cout << "No posts to show. Connect with people to see their posts!\n";
        }
    }

    void searchUsers(const std::string& query) {
        std::cout << "\n--- Search Results ---\n";
        bool found = false;
        for (const auto& pair : users) {
            // Simple search by username or full name
            if (pair.first.find(query) != std::string::npos || pair.second->getFullName().find(query) != std::string::npos) {
                std::cout << "- @" << pair.first << " (" << pair.second->getFullName() << ")\n";
                found = true;
            }
        }
        if (!found) {
            std::cout << "No users found matching your query.\n";
        }
    }
};

#endif // NETWORK_HPP