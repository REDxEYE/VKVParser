//
// Created by MED45 on 08.01.2022.
//

#ifndef VMTPARSER_VMT_PARSER_HPP
#define VMTPARSER_VMT_PARSER_HPP

#include <string>

#include "kv_lexer.hpp"
#include "kv_node.hpp"

using namespace std::literals;

namespace ValveKeyValueFormat {

    static TokenPair EmptyPeek = {TokenTypes::EMPTY, ""sv};

    class VmtParser {
        VmtLexer m_lexer;
        std::shared_ptr<VmtBranch> m_root = nullptr;

        TokenPair m_last_peek = EmptyPeek;

        std::vector<std::shared_ptr<VmtBranch>> m_node_stack;

        TokenPair peek();
        TokenPair advance();
        TokenPair expect(TokenTypes type);
        bool match(ValveKeyValueFormat::TokenTypes type, bool consume = false);

        inline void skip_newlines() {
            while (match(TokenTypes::NEWLINE, false) || match(TokenTypes::INVALID) || match(TokenTypes::COMMENT))
                advance();
        }

    public:
        explicit VmtParser(std::string_view buffer) : m_lexer(buffer){};
        void parse();
        inline std::shared_ptr<VmtNode> root() { return m_root; }
    };
}// namespace ValveKeyValueFormat


#endif//VMTPARSER_VMT_PARSER_HPP
