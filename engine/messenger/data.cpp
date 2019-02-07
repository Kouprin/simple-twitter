#include "data.h"

exec process_query(Messenger* messenger, std::vector<std::string>& query, std::vector<std::string>& answer) {
    std::cerr << query[0] << std::endl;
    answer.clear();
    if (query[0] == "addUser") {
        // q: addUser name email password
        if (query.size() != 4)
            return FAIL;
        return messenger->addUser(query[1], query[2], query[3]);
    }
    if (query[0] == "addChannel") {
        // q: addChannel name link
        if (query.size() != 3)
            return FAIL;
        return messenger->addChannel(query[1], query[2]);
    }
    if (query[0] == "addMessage") {
        // q: addMessage channelId userId message
        if (query.size() != 4)
            return FAIL;
        std::stringstream channel_stream(query[1]);
        int channel_id;
        channel_stream >> channel_id;
        std::stringstream user_stream(query[2]);
        int user_id;
        user_stream >> user_id;
        Message message(query[3], messenger->getUserPtr(user_id));
        return (messenger->getChannelPtr(channel_id))->addMessage(message);
    }
    if (query[0] == "getUser") {
        // q: getUser email
        // a: userId name
        if (query.size() != 2)
            return FAIL;
        User* user_ptr = nullptr;
        for (int i = 0; i < messenger->getUsersSize(); ++i) {
            if (messenger->getUserPtr(i)->getEmail() == query[1]) {
                // TODO build map<email, User*> instead
                user_ptr = messenger->getUserPtr(i);
                break;
            }
        }
        if (!user_ptr)
            return FAIL;
        answer.push_back(std::to_string(user_ptr->getId()));
        answer.push_back(user_ptr->getEmail());
        return SUCCESS;
    }
    if (query[0] == "loginUser") {
        // q: loginUser email password
        if (query.size() != 3)
            return FAIL;
        User* user_ptr = nullptr;
        for (int i = 0; i < messenger->getUsersSize(); ++i) {
            if (messenger->getUserPtr(i)->getEmail() == query[1]) {
                user_ptr = messenger->getUserPtr(i);
                break;
            }
        }
        if (!user_ptr)
            return FAIL;
        return user_ptr->checkPassword(query[2]);
    }
    if (query[0] == "getChannelByLink") {
        // q: getChannelByLink channelLink
        // a: id name
        if (query.size() != 2)
            return FAIL;
        Channel* channel_ptr = nullptr;
        for (int i = 0; i < messenger->getChannelsSize(); ++i) {
            if (messenger->getChannelPtr(i)->getLink() == query[1]) {
                // TODO build map<link, Channel*> instead
                channel_ptr = messenger->getChannelPtr(i);
                break;
            }
        }
        if (!channel_ptr)
            return FAIL;
        answer.push_back(std::to_string(channel_ptr->getId()));
        answer.push_back(channel_ptr->getName());
        return SUCCESS;
    }
    if (query[0] == "getMessages") {
        // q: getMessages channelId
        // a: count user_1 msg_1 user_2 msg_2 ... user_N msg_N
        if (query.size() != 2)
            return FAIL;
        std::stringstream channel_stream(query[1]);
        int channel_id;
        channel_stream >> channel_id;
        Channel* channel_ptr = messenger->getChannelPtr(channel_id);
        std::vector<Message> messages = channel_ptr->getMessages();
        answer.push_back(std::to_string(messages.size()));
        for (int i = 0; i < messages.size(); ++i) {
            answer.push_back(messages[i].getUserPtr()->getName());
            answer.push_back(messages[i].getText());
        }
        return SUCCESS;
    }
    return FAIL;
}

exec write_query(const char* buf, size_t len, std::string filename) {
    std::fstream fs;
    fs.open(filename, std::fstream::out | std::fstream::app);
    fs << buf << std::endl; // TODO len
    fs.close();
    return SUCCESS;
}

exec read_queries(Messenger* messenger, std::string filename) {
    std::fstream fs;
    fs.open(filename, std::fstream::in);
    for (std::string line; std::getline(fs, line); ) {
        std::vector<std::string> query, answer;
        parse_query(line.c_str(), line.size(), query);
        process_query(messenger, query, answer);
    }
    fs.close();
    return SUCCESS;
}
