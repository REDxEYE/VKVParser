//
// Created by MED45 on 07.01.2022.
//

#ifndef VMTPARSER_VMT_NODE_HPP
#define VMTPARSER_VMT_NODE_HPP
#include <memory>
#include <string>
#include <unordered_map>

enum class VmtNodeType {
    INVALID = -1,
    LEAF = 0,
    BRANCH = 1,

};

class VmtNode;
class VmtLeaf;
class VmtBranch;

class VmtNode {
    VmtNodeType type = VmtNodeType::INVALID;

public:
    std::string_view key;

    inline VmtNodeType get_type() const {
        return type;
    };
    inline const std::string_view& get_key() const {
        return key;
    };

    VmtLeaf* as_leaf();
    VmtBranch* as_branch();

    VmtNode(std::string_view key, VmtNodeType type) : type(type), key(key){};
};


class VmtLeaf : public VmtNode {
public:
    std::string_view value;
    std::string_view condition;

    VmtLeaf(std::string_view key, std::string_view value) : value(value), VmtNode(key, VmtNodeType::LEAF){};
    VmtLeaf(std::string_view key, std::string_view value, std::string_view condition) : value(value), condition(condition), VmtNode(key, VmtNodeType::LEAF){};
};

class VmtBranch : public VmtNode {
public:
    std::unordered_map<std::string_view, std::shared_ptr<VmtNode>> branches;
    std::string_view condition;

    inline void add_branch(const std::shared_ptr<VmtNode>& node) {
        branches.emplace(node->key, node);
    }

    std::shared_ptr<VmtNode> get(std::string_view key) {
        if (branches.find(key) == branches.end()) { return nullptr; }
        return branches.at(key);
    }

    explicit VmtBranch(std::string_view key) : VmtNode(key, VmtNodeType::BRANCH){};
    VmtBranch(std::string_view key, std::string_view condition) : condition(condition), VmtNode(key, VmtNodeType::LEAF){};
};


#endif//VMTPARSER_VMT_NODE_HPP
