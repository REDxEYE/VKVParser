//
// Created by MED45 on 08.01.2022.
//

#ifndef KVPARSER_KV_PARSER_HPP
#define KVPARSER_KV_PARSER_HPP

#include <string>

#include "kv_lexer.hpp"
#include "kv_node.hpp"

using namespace std::literals;

namespace ValveKeyValueFormat {

    static TokenPair EmptyPeek = {TokenTypes::EMPTY, ""sv};

    class KVParser {
        KVLexer m_lexer;
        std::shared_ptr<KVBranch> m_root = nullptr;

        TokenPair m_last_peek = EmptyPeek;

        std::vector<std::shared_ptr<KVBranch>> m_node_stack;

        TokenPair peek();
        TokenPair advance();
        TokenPair expect(TokenTypes type);
        bool match(ValveKeyValueFormat::TokenTypes type, bool consume = false);

        inline void skip_newlines() {
            while (match(TokenTypes::NEWLINE, false) || match(TokenTypes::INVALID) || match(TokenTypes::COMMENT))
                advance();
        }
        inline void skip_comments() {
            while (match(TokenTypes::INVALID) || match(TokenTypes::COMMENT))
                advance();
        }
        inline void try_to_recover() {
            while (true) {
                if (match(TokenTypes::NEWLINE)) break;
                if (match(TokenTypes::END_OF_FILE)) break;
                advance();
            };
        }

    public:
        explicit KVParser(std::string_view buffer) : m_lexer(buffer){};
        void parse();
        inline std::shared_ptr<KVNode> root() { return m_root; }
    };
}// namespace ValveKeyValueFormat


#endif//KVPARSER_KV_PARSER_HPP
