//
// Created by MED45 on 07.01.2022.
//

#include "VKVParser/kv_node.hpp"
ValveKeyValueFormat::KVLeaf* ValveKeyValueFormat::KVNode::as_leaf() {
    if (type == KVNodeType::LEAF) return static_cast<KVLeaf*>(this);
    return nullptr;
}

ValveKeyValueFormat::KVBranch* ValveKeyValueFormat::KVNode::as_branch() {
    if (type == KVNodeType::BRANCH) return static_cast<KVBranch*>(this);
    return nullptr;
}