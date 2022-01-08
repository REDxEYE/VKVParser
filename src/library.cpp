#include "library.h"
#include <cstdio>
#include <vmt_parser.hpp>

LoggerFunction* logger_function = [](const std::string& message, LogLevel severity) -> void { fprintf(stderr, "%s\n", message.c_str()); };

std::shared_ptr<VmtNode> parseVmtBuffer(const std::string& buffer) {
    auto parser = ValveKeyValueFormat::VmtParser(buffer);
    parser.parse();
    return parser.root();
}
