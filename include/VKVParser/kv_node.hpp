//
// Created by MED45 on 07.01.2022.
//

#ifndef KVPARSER_KV_NODE_HPP
#define KVPARSER_KV_NODE_HPP
#include <memory>
#include <string>
#include <unordered_map>

enum class KVNodeType {
    INVALID = -1,
    LEAF = 0,
    BRANCH = 1,

};

class KVNode;
class KVLeaf;
class KVBranch;

class KVNode {
    KVNodeType type = KVNodeType::INVALID;

public:
    std::string_view key;

    inline KVNodeType get_type() const {
        return type;
    };
    inline const std::string_view& get_key() const {
        return key;
    };

    KVLeaf* as_leaf();
    KVBranch* as_branch();

    KVNode(std::string_view key, KVNodeType type) : type(type), key(key){};
};


class KVLeaf : public KVNode {
public:
    std::string_view value;
    std::string_view condition;

    KVLeaf(std::string_view key, std::string_view value) : value(value), KVNode(key, KVNodeType::LEAF){};
    KVLeaf(std::string_view key, std::string_view value, std::string_view condition) : value(value), condition(condition), KVNode(key, KVNodeType::LEAF){};
};

class KVBranch : public KVNode {
public:
    std::unordered_map<std::string_view, std::shared_ptr<KVNode>> branches;
    std::string_view condition;

    inline void add_branch(const std::shared_ptr<KVNode>& node) {
        branches.emplace(node->key, node);
    }

    std::shared_ptr<KVNode> get(std::string_view key) {
        if (branches.find(key) == branches.end()) { return nullptr; }
        return branches.at(key);
    }

    explicit KVBranch(std::string_view key) : KVNode(key, KVNodeType::BRANCH){};
    KVBranch(std::string_view key, std::string_view condition) : condition(condition), KVNode(key, KVNodeType::LEAF){};
};


#endif//KVPARSER_KV_NODE_HPP
