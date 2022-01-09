//
// Created by MED45 on 08.01.2022.
//

#include "VKVParser/kv_parser.hpp"
#include "VKVParser/shared.hpp"
#include <sstream>
ValveKeyValueFormat::TokenPair ValveKeyValueFormat::KVParser::peek() {
    if (m_last_peek.first == TokenTypes::EMPTY) {
        m_last_peek = m_lexer.next_token();
		std::stringstream log_msg;
		log_msg<<"TokenID "<<((uint32_t) m_last_peek.first)<<" token value: \""<<m_last_peek.second<<"\"";
        logger_function(log_msg.str(), LogLevel::TRACE);
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
	std::stringstream log_msg;
	log_msg<<"TokenID "<<((uint32_t) m_last_peek.first)<<" token value: \""<<m_last_peek.second<<"\"";
    logger_function(log_msg.str(), LogLevel::TRACE);
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
		std::stringstream log_msg;
		log_msg<<"Trying to recover from unexpected token "<<( (uint32_t) token.first)<<":\""<<token.second<<"\", expected "<<((uint32_t) type)
			<<" at "<<m_lexer.m_line<<":"<<m_lexer.m_column;
        logger_function(log_msg.str(),
                        LogLevel::WARN);
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
                    logger_function("Missing new line!", LogLevel::ERROR);
                }
            } else if (match(TokenTypes::STRING)) {
                auto value = advance();
                if (match(TokenTypes::LBRACKET, true)) {
                    auto condition = expect(TokenTypes::STRING);
                    if (condition.first == TokenTypes::INVALID) break;
                    if (expect(TokenTypes::RBRACKET).first == TokenTypes::INVALID) break;

                    auto leaf = std::make_shared<KVLeaf>(key.second, value.second, condition.second);
                    m_node_stack.back()->add_branch(leaf);
                } else {
                    auto leaf = std::make_shared<KVLeaf>(key.second, value.second);
                    m_node_stack.back()->add_branch(leaf);
                }
                skip_comments();
                if (expect(TokenTypes::NEWLINE).first == TokenTypes::INVALID) {
                    logger_function("Missing new line!", LogLevel::ERROR);
                }
            }
        } else if (match(TokenTypes::RBRACE, true)) {
            m_node_stack.pop_back();
        } else if (match(TokenTypes::END_OF_FILE))
            break;
        else {
            auto unexpected = advance();
			std::stringstream log_msg;
			log_msg<<"Unexpected token "<<((uint32_t) unexpected.first)<<":\""<<unexpected.second<<"\" at "<<m_lexer.m_line<<":"<<m_lexer.m_column;
            logger_function(log_msg.str(), LogLevel::WARN);
            try_to_recover();
        }
    }
}
