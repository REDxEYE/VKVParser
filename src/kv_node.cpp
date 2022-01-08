//
// Created by MED45 on 07.01.2022.
//

#include "vmt_node.hpp"
VmtLeaf* VmtNode::as_leaf() {
    if (type == VmtNodeType::LEAF) return static_cast<VmtLeaf*>(this);
    return nullptr;
}

VmtBranch* VmtNode::as_branch() {
    if (type == VmtNodeType::BRANCH) return static_cast<VmtBranch*>(this);
    return nullptr;
}