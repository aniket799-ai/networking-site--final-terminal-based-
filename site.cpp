#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace std;

// --- 1. Base Class: Profile (Uses Inheritance) ---

/**
 * The abstract base class for all professionals.
 */
class Profile {
protected:
    int profileID;
    string name;
    string profession;
    vector<shared_ptr<Profile>> connections;

public:
    // Constructor
    Profile(int id, const string& n, const string& p) : profileID(id), name(n), profession(p) {}

    // Virtual destructor is crucial for proper memory management with inheritance
    virtual ~Profile() {}

    // Pure virtual function (makes Profile an Abstract Class)
    virtual void introduce() const = 0;

    // Common method for all profiles
    void addConnection(shared_ptr<Profile> newConnection) {
        // Check if already connected (simple ID check)
        auto it = find_if(connections.begin(), connections.end(), 
                          [&](const shared_ptr<Profile>& conn){
                              return conn->getID() == newConnection->getID();
                          });

        if (it == connections.end() && this->profileID != newConnection->getID()) {
            connections.push_back(newConnection);
            cout << name << " connected with " << newConnection->getName() << "." << endl;
        }
    }

    // Accessors
    int getID() const { return profileID; }
    string getName() const { return name; }
    string getProfession() const { return profession; }

    // Display connections
    void displayConnections() const {
        cout << "\n--- Connections of " << name << " (" << profession << ") ---" << endl;
        if (connections.empty()) {
            cout << "No connections yet." << endl;
            return;
        }
        for (const auto& conn : connections) {
            cout << "- ID: " << conn->getID() << ", Name: " << conn->getName() << ", Field: " << conn->getProfession() << endl;
        }
    }
};

// --- 2. Derived Classes (Demonstrating Inheritance) ---

class Engineer : public Profile {
private:
    string specialization;
public:
    Engineer(int id, const string& n, const string& spec)
        : Profile(id, n, "Engineer"), specialization(spec) {}

    // Polymorphic implementation of introduce()
    void introduce() const override {
        cout << "Hello, I am " << name << ", an " << profession 
             << " specializing in " << specialization << "." << endl;
    }
};

class Doctor : public Profile {
private:
    string medicalField;
public:
    Doctor(int id, const string& n, const string& field)
        : Profile(id, n, "Doctor"), medicalField(field) {}

    // Polymorphic implementation of introduce()
    void introduce() const override {
        cout << "Greetings, I am Dr. " << name << ", a " << profession 
             << " working in " << medicalField << "." << endl;
    }
};

class Artist : public Profile {
private:
    string medium;
public:
    Artist(int id, const string& n, const string& m)
        : Profile(id, n, "Artist"), medium(m) {}

    // Polymorphic implementation of introduce()
    void introduce() const override {
        cout << "Hi, I'm " << name << ", an " << profession 
             << ". My primary medium is " << medium << "." << endl;
    }
};


// --- 3. Interaction Components (Post & Comment Classes) ---

/**
 * Class to represent a comment on a Post.
 */
class Comment {
private:
    string content;
    string authorName;
public:
    Comment(const string& c, const string& a) : content(c), authorName(a) {}

    void display() const {
        cout << "    [Comment by " << authorName << "]: " << content << endl;
    }
};

/**
 * Class to represent a post made by a professional.
 */
class Post {
private:
    int postID;
    string content;
    string authorName;
    string authorProfession;
    vector<Comment> comments;

public:
    Post(int id, const string& c, const string& n, const string& p)
        : postID(id), content(c), authorName(n), authorProfession(p) {}
    
    // <<< --- FIX APPLIED HERE --- >>>
    // We need a public accessor to read the private member postID.
    int getPostID() const { return postID; } 

    void addComment(const string& content, const string& authorName) {
        comments.emplace_back(content, authorName);
    }

    void display() const {
        cout << "\n=========================================" << endl;
        cout << "POST ID: " << postID << endl;
        cout << "Author: " << authorName << " (" << authorProfession << ")" << endl;
        cout << "Content: " << content << endl;
        cout << "--- Comments (" << comments.size() << ") ---" << endl;
        if (comments.empty()) {
            cout << "  No comments yet." << endl;
        } else {
            for (const auto& c : comments) {
                c.display();
            }
        }
        cout << "=========================================" << endl;
    }
};


// --- 4. Network Management Class (Encapsulation) ---

/**
 * The central class that manages all profiles and posts.
 * It encapsulates the entire data store.
 */
class SocialNetwork {
private:
    vector<shared_ptr<Profile>> allProfiles;
    vector<Post> allPosts;
    int nextProfileID = 101;
    int nextPostID = 1;

public:
    // Factory method to create and add profiles
    shared_ptr<Profile> createProfile(const string& type, const string& name, const string& detail) {
        shared_ptr<Profile> newProfile;
        if (type == "Engineer") {
            newProfile = make_shared<Engineer>(nextProfileID++, name, detail);
        } else if (type == "Doctor") {
            newProfile = make_shared<Doctor>(nextProfileID++, name, detail);
        } else if (type == "Artist") {
            newProfile = make_shared<Artist>(nextProfileID++, name, detail);
        } else {
            cout << "Invalid profession type." << endl;
            return nullptr;
        }
        allProfiles.push_back(newProfile);
        cout << "Profile created for " << name << " (ID: " << newProfile->getID() << ")." << endl;
        return newProfile;
    }

    // Method to create a new Post
    void createPost(shared_ptr<Profile> author, const string& content) {
        if (!author) return;
        allPosts.emplace_back(nextPostID++, content, author->getName(), author->getProfession());
        cout << "\nPost created successfully by " << author->getName() << "." << endl;
    }

    // Method to add a comment to a Post
    void addComment(int postID, shared_ptr<Profile> commenter, const string& content) {
        if (!commenter) return;
        for (auto& post : allPosts) {
            // <<< --- FIX IS USED HERE --- >>>
            if (post.getPostID() == postID) { 
                post.addComment(content, commenter->getName());
                cout << "Comment added to Post ID " << postID << " by " << commenter->getName() << "." << endl;
                return;
            }
        }
        cout << "Error: Post ID " << postID << " not found." << endl;
    }
    
    // Method to find a profile by ID
    shared_ptr<Profile> getProfileByID(int id) {
        for (const auto& p : allProfiles) {
            if (p->getID() == id) {
                return p;
            }
        }
        return nullptr;
    }

    // Display all posts
    void displayFeed() const {
        cout << "\n\n=========================================" << endl;
        cout << "         PROFESSIONAL FEED" << endl;
        cout << "=========================================" << endl;
        if (allPosts.empty()) {
            cout << "The feed is empty." << endl;
            return;
        }
        for (const auto& post : allPosts) {
            post.display();
        }
    }

    // Display all profiles (for connection look-up)
    void displayAllProfiles() const {
        cout << "\n--- All Network Profiles ---" << endl;
        for (const auto& p : allProfiles) {
            cout << "ID: " << p->getID() << ", Name: " << p->getName() << ", Profession: " << p->getProfession() << endl;
        }
    }
};

// --- 5. Main Execution and Demonstration ---

int main() {
    // Enable fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "--- Professional Social Network Simulation (C++ OOPS) ---" << endl;
    
    // Create the network manager object
    SocialNetwork net; 

    // 1. OBJECT CREATION (Profiles)
    cout << "\n--- Creating Professional Profiles (Objects) ---" << endl;
    
    shared_ptr<Profile> eng1 = net.createProfile("Engineer", "Alice Johnson", "AI Development");
    shared_ptr<Profile> doc1 = net.createProfile("Doctor", "Bob Williams", "Cardiology");
    shared_ptr<Profile> art1 = net.createProfile("Artist", "Clara Smith", "Digital Painting");
    shared_ptr<Profile> eng2 = net.createProfile("Engineer", "David Lee", "Mechanical Design");
    
    // 2. POLYMORPHISM Demonstration
    cout << "\n--- Introducing Professionals (Polymorphism) ---" << endl;
    if (eng1) eng1->introduce();
    if (doc1) doc1->introduce();
    if (art1) art1->introduce();

    // 3. CONNECTION (Interaction)
    cout << "\n--- Establishing Connections ---" << endl;
    if (eng1 && doc1) eng1->addConnection(doc1);
    if (eng1 && art1) eng1->addConnection(art1);
    if (doc1 && eng2) doc1->addConnection(eng2);

    // 4. POSTING (Creating Interaction Objects)
    net.createPost(eng1, "Just finished a new framework for neural network training!");
    net.createPost(doc1, "A quick guide on preventing burnout for young professionals.");
    net.createPost(art1, "My latest piece: 'The Logic Gate'.");
    
    // 5. COMMENTING (Object Interaction)
    net.addComment(1, doc1, "Great work, Alice! How's the performance optimization?");
    net.addComment(3, eng1, "Fantastic use of light and shadow, Clara.");
    net.addComment(2, eng2, "Very helpful advice, Dr. Williams!");

    // 6. FEED DISPLAY & REVIEW
    net.displayFeed();
    
    // 7. REVIEW CONNECTIONS
    if (eng1) eng1->displayConnections();
    
    return 0;
}