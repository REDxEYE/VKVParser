//
// Created by MED45 on 07.01.2022.
//

#ifndef KVPARSER_KV_LEXER_HPP
#define KVPARSER_KV_LEXER_HPP

#include "kv_node.hpp"
#include <string>
#include <vector>

namespace ValveKeyValueFormat {
    class KVParser;
    enum class TokenTypes {
        EMPTY = -1,
        INVALID = 0,
        STRING = 1,
        NUMERIC = 2,
        IDENTIFIER = 3,
        COMMENT = 4,
        LPAREN = 5,
        RPAREN = 7,
        LBRACKET = 7,
        RBRACKET = 8,
        LBRACE = 9,
        RBRACE = 10,
        NEWLINE = 11,
        END_OF_FILE = 12,
    };

    using TokenPair = std::pair<ValveKeyValueFormat::TokenTypes, std::string_view>;

    class KVLexer {
    private:
        std::string_view m_buffer;
        uint32_t m_offset = 0;
        uint32_t m_line = 0;
        uint32_t m_column = 0;



        inline char symbol() {
            if (m_offset >= m_buffer.size()) return 0;
            return m_buffer[m_offset];
        }
        inline char next_symbol() {
            if (m_offset + 1 >= m_buffer.size()) return 0;
            return m_buffer[m_offset + 1];
        };
        inline char advance();

        inline std::string_view read_simple_string(std::string_view terminators);
        inline std::string_view read_quoted_string();
        friend KVParser;

    public:
        explicit KVLexer(const std::string& buffer) {
            this->m_buffer = std::string_view(buffer);
        }
        explicit KVLexer(std::string_view buffer) {
            this->m_buffer = buffer;
        }

        TokenPair next_token();
    };
}// namespace ValveKeyValueFormat


#endif//KVPARSER_KV_LEXER_HPP
