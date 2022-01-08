#ifndef VMTPARSER_LIBRARY_H
#define VMTPARSER_LIBRARY_H
#include <memory>
#include <string>

#include "shared.hpp"
#include "vmt_node.hpp"

std::shared_ptr<VmtNode> parseVmtBuffer(const std::string& buffer);
void setLogCallback(LoggerFunction* loggerFunction);

#endif//VMTPARSER_LIBRARY_H
