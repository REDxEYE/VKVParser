//
// Created by MED45 on 07.01.2022.
//

#ifndef KVPARSER_SHARED_HPP
#define KVPARSER_SHARED_HPP

enum class LogLevel {
    ALL = 0,
    TRACE = 1,
    DEBUG = 2,
    WARN = 3,
    ERR = 4,
};

typedef void LoggerFunction(const std::string& message, LogLevel severity);
extern LoggerFunction* logger_function;


inline std::string_view trim(std::string_view in) {
    uint32_t left = 0;
    for (;; ++left) {
        if (left == in.length())
            return {};
        if (!isspace(in[left]))
            break;
    }
    auto right = in.length();
    for (; right > left && isspace(in[right-1]); --right) {};

    return in.substr(left, right-left);
}

#ifdef _WIN32
#    define LIBRARY_API __declspec(dllexport)
#elif
#    define LIBRARY_API
#endif

#endif//KVPARSER_SHARED_HPP
