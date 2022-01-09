#include "VKVParser/library.h"
#include <VKVParser/kv_parser.hpp>
#include <cstdio>

ValveKeyValueParser::LoggerFunction* logger_function = [](const std::string& message, ValveKeyValueParser::LogLevel severity) -> void { fprintf(stderr, "%s\n", message.c_str()); };

LIBRARY_API std::shared_ptr<KVNode> parseKVBuffer(const std::string& buffer) {
    auto parser = ValveKeyValueFormat::KVParser(buffer);
    parser.parse();
    return parser.root();
}
LIBRARY_API void setLogCallback(ValveKeyValueParser::LoggerFunction* loggerFunction) {
    ValveKeyValueParser::logger_function = loggerFunction;
}
