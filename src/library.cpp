#include "library.h"
#include <cstdio>
#include <kv_parser.hpp>

LoggerFunction* logger_function = [](const std::string& message, LogLevel severity) -> void { fprintf(stderr, "%s\n", message.c_str()); };

std::shared_ptr<KVNode> parseKVBuffer(const std::string& buffer) {
    auto parser = ValveKeyValueFormat::KVParser(buffer);
    parser.parse();
    return parser.root();
}
