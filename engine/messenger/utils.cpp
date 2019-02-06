#include "utils.h"

exec parse_query(const char* buf, size_t len, std::vector<std::string>& query) {
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
        std::cerr << e.what() << std::endl;
    }
    return FAIL;
}

exec process(std::vector<std::string>& query) {
    std::cerr << query[0] << std::endl;
    return SUCCESS;
}
