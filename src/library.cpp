#include <cstdio>
#include "VKVParser/library.h"
#include <VKVParser/kv_parser.hpp>

ValveKeyValueFormat::LoggerFunction* logger_function = [](const std::string& message, ValveKeyValueFormat::LogLevel severity) -> void { fprintf(stderr, "%s\n", message.c_str()); };

LIBRARY_API std::shared_ptr<KVNode> parseKVBuffer(const std::string& buffer) {
    auto parser = ValveKeyValueFormat::KVParser(buffer);
    parser.parse();
    return parser.root();
}
LIBRARY_API void setLogCallback(ValveKeyValueFormat::LoggerFunction* loggerFunction) {
    ValveKeyValueFormat::logger_function = loggerFunction;
}
