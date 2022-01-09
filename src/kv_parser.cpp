//
// Created by MED45 on 08.01.2022.
//

#include "VKVParser/kv_parser.hpp"
#include <algorithm>
#include <format>
#include <string>
ValveKeyValueFormat::TokenPair ValveKeyValueFormat::KVParser::peek() {
    if (m_last_peek.first == TokenTypes::EMPTY) {
        m_last_peek = m_lexer.next_token();
        ValveKeyValueParser::logger_function(std::format("TokenID {} token value: \"{}\"", (uint32_t) m_last_peek.first, m_last_peek.second), ValveKeyValueParser::LogLevel::TRACE);
        fflush(stdout);
    }
    return m_last_peek;
}
ValveKeyValueFormat::TokenPair ValveKeyValueFormat::KVParser::advance() {
    if (m_last_peek.first != TokenTypes::EMPTY) {
        auto result = m_last_peek;
        m_last_peek = EmptyPeek;
        return result;
    }
    auto token = m_lexer.next_token();
    ValveKeyValueParser::logger_function(std::format("TokenID {} token value: \"{}\"", (uint32_t) m_last_peek.first, m_last_peek.second), ValveKeyValueParser::LogLevel::TRACE);
    fflush(stdout);
    return token;
}
bool ValveKeyValueFormat::KVParser::match(ValveKeyValueFormat::TokenTypes type, bool consume) {
    auto token = peek();
    if (token.first != type) return false;
    if (consume) advance();
    return true;
}
ValveKeyValueFormat::TokenPair ValveKeyValueFormat::KVParser::expect(ValveKeyValueFormat::TokenTypes type) {
    auto token = peek();
    if (token.first != type) {
        ValveKeyValueParser::logger_function(std::format("Trying to recover from unexpected token {}:\"{}\", expected {} at {}:{}",
                                    (uint32_t) token.first, token.second, (uint32_t) type, m_lexer.m_line, m_lexer.m_column),
                        ValveKeyValueParser::LogLevel::WARN);
        try_to_recover();
        return {TokenTypes::INVALID, ""sv};
    }
    advance();
    return token;
}
void ValveKeyValueFormat::KVParser::parse() {
    while (!match(TokenTypes::END_OF_FILE)) {
        skip_newlines();
        if (match(TokenTypes::STRING)) {
            auto key = advance();
            std::string tkey = std::string(key.second);
            ValveKeyValueParser::to_lower(tkey);

            skip_newlines();
            if (match(TokenTypes::LBRACE, true)) {
                auto new_branch = std::make_shared<KVBranch>(key.second);
                if (m_root == nullptr) m_root = new_branch;
                else {
                    m_node_stack.back()->add_branch(new_branch);
                }
                m_node_stack.emplace_back(new_branch);
                skip_comments();
                if (expect(TokenTypes::NEWLINE).first == TokenTypes::INVALID) {
                    ValveKeyValueParser::logger_function("Missing new line!", ValveKeyValueParser::LogLevel::ERROR);
                }
            } else if (match(TokenTypes::STRING)) {
                auto value = advance();
                if (match(TokenTypes::LBRACKET, true)) {
                    auto condition = expect(TokenTypes::STRING);
                    if (condition.first == TokenTypes::INVALID) break;
                    if (expect(TokenTypes::RBRACKET).first == TokenTypes::INVALID) break;

                    auto leaf = std::make_shared<KVLeaf>(tkey, value.second, condition.second);
                    m_node_stack.back()->add_branch(leaf);
                } else {
                    auto leaf = std::make_shared<KVLeaf>(tkey, value.second);
                    m_node_stack.back()->add_branch(leaf);
                }
                skip_comments();
                if (expect(TokenTypes::NEWLINE).first == TokenTypes::INVALID) {
                    ValveKeyValueParser::logger_function("Missing new line!", ValveKeyValueParser::LogLevel::ERROR);
                }
            }
        } else if (match(TokenTypes::RBRACE, true)) {
            m_node_stack.pop_back();
        } else if (match(TokenTypes::END_OF_FILE))
            break;
        else {
            auto unexpected = advance();
            ValveKeyValueParser::logger_function(std::format("Unexpected token {}:\"{}\" at {}:{}", (uint32_t) unexpected.first, unexpected.second, m_lexer.m_line, m_lexer.m_column), ValveKeyValueParser::LogLevel::WARN);
            try_to_recover();
        }
    }
}
