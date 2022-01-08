//
// Created by MED45 on 08.01.2022.
//

#include "kv_parser.hpp"
#include "shared.hpp"
#include <format>
ValveKeyValueFormat::TokenPair ValveKeyValueFormat::KVParser::peek() {
    if (m_last_peek.first == TokenTypes::EMPTY) {
        m_last_peek = m_lexer.next_token();
        printf("TokenID %i token value: \"%s\"\n", m_last_peek.first, std::string(m_last_peek.second).c_str());
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
    printf("TokenID %i token value: \"%s\"\n", token.first, std::string(token.second).c_str());
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
        logger_function(std::format("Trying to recover from unexpected token {}:\"{}\", expected {} at {}:{}",
                                    (uint32_t) token.first, token.second, (uint32_t) type, m_lexer.m_line, m_lexer.m_column),
                        LogLevel::WARN);
        while (!match(TokenTypes::NEWLINE) || match(TokenTypes::END_OF_FILE)) advance();
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
            skip_newlines();
            if (match(TokenTypes::LBRACE, true)) {
                auto new_branch = std::make_shared<KVBranch>(key.second);
                if (m_root == nullptr) m_root = new_branch;
                else {
                    m_node_stack.back()->add_branch(new_branch);
                }
                m_node_stack.emplace_back(new_branch);
            } else if (match(TokenTypes::STRING)) {
                auto value = advance();
                if (match(TokenTypes::STRING)) {
                    auto condition = expect(TokenTypes::STRING);
                    if (condition.first == TokenTypes::INVALID) break;

                    auto leaf = std::make_shared<KVLeaf>(key.second, value.second, condition.second);
                    m_node_stack.back()->add_branch(leaf);
                } else {
                    auto leaf = std::make_shared<KVLeaf>(key.second, value.second);
                    m_node_stack.back()->add_branch(leaf);
                }
            }
        } else if (match(TokenTypes::RBRACE, true)) {
            m_node_stack.pop_back();
        } else if (match(TokenTypes::END_OF_FILE))
            break;
        else {
            auto unexpected = advance();
            logger_function(std::format("Unexpected token {}:\"{}\" at {}:{}", (uint32_t) unexpected.first, unexpected.second, m_lexer.m_line, m_lexer.m_column), LogLevel::ERROR);
            break;
        }
    }
}
