#pragma once

#include <iostream>
#include <ctime>
#include <boost/asio.hpp>

enum exec {
    UNKNOWN_FAIL,
    SUCCESS,
    FAIL
};

exec parse_query(const char* buf, size_t len, std::vector<std::string>& query);
exec process(std::vector<std::string>& query);
