#pragma once

#include <iostream>
#include <ctime>
#include <sstream>
#include <boost/asio.hpp>

enum exec {
    UNKNOWN_FAIL,
    SUCCESS,
    FAIL
};

exec parseQuery(const char* buf, size_t len, std::vector<std::string>& query);
