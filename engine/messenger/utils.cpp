#include "utils.h"

exec parseQuery(const char* buf, size_t len, std::vector<std::string>& query) {
    try {
        std::stringstream ss;
        ss << buf;
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

