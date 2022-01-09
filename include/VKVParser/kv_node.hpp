//
// Created by MED45 on 07.01.2022.
//

#ifndef KVPARSER_KV_NODE_HPP
#define KVPARSER_KV_NODE_HPP
#include "shared.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

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
    std::string key;

    inline KVNodeType get_type() const {
        return type;
    };
    inline const std::string& get_key() const {
        return key;
    };

    KVLeaf* as_leaf();
    KVBranch* as_branch();
    KVNode(std::string_view key, KVNodeType type) : KVNode(std::string(key), type){};
    KVNode(std::string key, KVNodeType type) : type(type), key(std::move(key)){};
};


class KVLeaf : public KVNode {
public:
    std::string value;
    std::string condition;

    KVLeaf(std::string_view key, std::string_view value) : KVLeaf(std::string(key), std::string(value)){};
    KVLeaf(std::string_view key, std::string_view value, std::string_view condition) : KVLeaf(std::string(key), std::string(value), std::string(condition)){};
    KVLeaf(std::string key, std::string value) : value(std::move(value)), KVNode(std::move(key), KVNodeType::LEAF){};
    KVLeaf(std::string key, std::string value, std::string condition) : value(std::move(value)), condition(std::move(condition)), KVNode(std::move(key), KVNodeType::LEAF){};
};

class KVBranch : public KVNode {
public:
    std::unordered_map<std::string_view, std::shared_ptr<KVNode>> branches;
    std::string condition;

    inline void add_branch(const std::shared_ptr<KVNode>& node) {
        branches.emplace(node->key, node);
    }

    std::shared_ptr<KVNode> get(const std::string& key) {
        std::string normalized_key = key;
        normalized_key = to_lower(normalized_key);
        auto result = branches.find(normalized_key);
        if (result == branches.end()) { return nullptr; }
        return result->second;
    }

    explicit KVBranch(std::string_view key) : KVBranch(std::string(key)){};
    KVBranch(std::string_view key, std::string_view condition) : KVBranch(std::string(key), std::string(condition)){};

    explicit KVBranch(std::string key) : KVNode(std::move(key), KVNodeType::BRANCH){};
    KVBranch(std::string key, std::string condition) : condition(std::move(condition)), KVNode(std::move(key), KVNodeType::LEAF){};
};


#endif//KVPARSER_KV_NODE_HPP
