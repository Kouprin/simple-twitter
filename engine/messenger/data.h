#pragma once

#include <chrono>
#include <experimental/random>
#include <fstream>
#include "utils.h"

class User;

class Message {
    private:
        std::string text;
        User* user;

    public:
        Message(std::string& text, User* user) {
            this->text = text;
            this->user = user;
        }
        std::string getText() {
            return text;
        }
        User* getUserPtr() {
            return user;
        }
};

class User {
    private:
        int id;
        std::string name;
        std::string email;
        std::string password;

    public:
        User(int id, std::string& name, std::string& email, std::string& password) {
            this->id = id;
            this->name = name;
            this->email = email;
            this->password = password;
        }
        int getId() {
            return id;
        }
        std::string getName() {
            return name;
        }
        std::string getEmail() {
            return email;
        }
        exec checkPassword(std::string& password) {
            return this->password == password ? SUCCESS : FAIL;
        }
};

class Channel {
    private:
        int id;
        std::string name;
        std::string link;
        std::vector<Message> messages;
        /*void generateLink() {
            assert(!name.empty());
            for (int i = 0; i < 6; ++i) {
                int random_number = std::experimental::randint(0, 25);
                char letter = 'a' + random_number;
                link = link + letter;
            }
        }*/

    public:
        Channel(int id, std::string& name, std::string& link) {
            this->id = id;
            this->name = name;
            this->link = link;
        }
        int getId() {
            return id;
        }
        int getMessagesSize() {
            return messages.size();
        }
        std::vector<Message> getMessages() {
            return messages;
        }
        std::string getLink() {
            return link;
        }
        std::string getName() {
            return name;
        }
        exec addMessage(Message& message) {
            messages.push_back(message);
            return SUCCESS;
        }

};

class Messenger {
    private:
        std::vector<Channel*> channels;
        std::vector<User*> users;

    public:
        Messenger() {}
        exec addChannel(std::string& channelName, std::string& link) {
            int id = channels.size();
            Channel* channel = new Channel(id, channelName, link);
            channels.push_back(channel);
            return SUCCESS;
        }
        exec addUser(std::string& name, std::string& email, std::string& password) {
            int id = users.size();
            User* user = new User(id, name, email, password);
            users.push_back(user);
            return SUCCESS;
        }
        User* getUserPtr(int index) {
            return users[index];
        }
        Channel* getChannelPtr(int index) {
            return channels[index];
        }
        int getChannelsSize() {
            return channels.size();
        }
        int getUsersSize() {
            return users.size();
        }
};

exec read_queries(Messenger* messenger, std::string filename);
exec write_query(const char* buf, size_t len, std::string filename);
exec process_query(Messenger* messenger, std::vector<std::string>& query, std::vector<std::string>& answer);
