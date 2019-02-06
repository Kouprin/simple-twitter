#include "utils.h"

exec parseQuery(const char* buf, size_t len, std::vector<std::string>& query) {
    try {
        std::stringstream ss;
        char copy[4097];
        strcpy(copy, buf);
        while (strlen(copy) && isspace(copy[strlen(copy) - 1])) {
            // TODO telnet quick fix
            copy[strlen(copy) - 1] = copy[strlen(copy)];
        }
        ss << copy;
        query.clear();
        std::string token;
        while (std::getline(ss, token, '\t')) {
            query.push_back(token);
        }
        if (query.empty()) {
            std::cerr << "Empty query" << std::endl;
            return FAIL;
        }
        return SUCCESS;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Unable to parse" << std::endl;
    }
    return FAIL;
}

