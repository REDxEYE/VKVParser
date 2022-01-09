#ifndef KVPARSER_LIBRARY_H
#define KVPARSER_LIBRARY_H
#include <memory>
#include <string>

#include "kv_node.hpp"
#include "shared.hpp"

LIBRARY_API std::shared_ptr<KVNode> parseKVBuffer(const std::string& buffer);
LIBRARY_API void setLogCallback(ValveKeyValueFormat::LoggerFunction* loggerFunction);

#endif//KVPARSER_LIBRARY_H
