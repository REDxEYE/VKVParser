#ifndef KVPARSER_LIBRARY_H
#define KVPARSER_LIBRARY_H
#include <memory>
#include <string>

#include "kv_node.hpp"
#include "shared.hpp"

std::shared_ptr<KVNode> parseKVBuffer(const std::string& buffer);
void setLogCallback(LoggerFunction* loggerFunction);

#endif//KVPARSER_LIBRARY_H
