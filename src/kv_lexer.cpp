//
// Created by MED45 on 07.01.2022.
//
#include <format>

#include "VKVParser/kv_lexer.hpp"

using namespace std::literals;


const std::string_view valid_escapes{"\\'\"ntr"};
const std::string_view invalid_unquoted_chars{"\"'"};

char ValveKeyValueFormat::KVLexer::advance() {
    char symbol = this->symbol();
    if (symbol) {
        if (symbol == '\r' && next_symbol() == '\n') m_offset++;
        if (symbol == '\n' || symbol == '\r') {
            m_line++;
            m_column = 1;
        } else {
            m_column++;
        }
        m_offset++;
    }
    return symbol;
}

std::string_view ValveKeyValueFormat::KVLexer::read_simple_string(std::string_view terminators) {
    uint32_t string_start = m_offset;
    while (true) {
        char symbol = this->symbol();
        if (terminators.find(symbol) != -1) break;
        if (invalid_unquoted_chars.find(symbol) != -1) {
            ValveKeyValueParser::logger_function(std::format("Unexpected char \"{}\" in unquoted string at {}:{}", symbol, m_line + 1, m_column), ValveKeyValueParser::LogLevel::WARN);
            break;
        }
        if (!isprint(symbol) || symbol == '[' || symbol == ']' || symbol == '{' || symbol == '}') break;
        advance();
    }

    auto view = m_buffer.substr(string_start, m_offset - string_start);
    return ValveKeyValueParser::trim(view);
}

std::string_view ValveKeyValueFormat::KVLexer::read_quoted_string() {
    char terminator = advance();
    uint32_t string_start = m_offset;
    uint32_t string_end = m_offset;
    while (true) {
        char symbol = this->symbol();
        if (symbol == terminator || symbol == '\n') break;
        if (symbol == '\\' && valid_escapes.find(next_symbol()) != -1) { advance(); }
        if (!isprint(symbol)) break;
        advance();
        string_end++;
    }

    if (symbol() != terminator) {
        if (symbol() != '\n') advance();
        ValveKeyValueParser::logger_function(std::format(R"(Expected "{}", but got "{}" at {}:{})", terminator, symbol(), m_line + 1, m_column), ValveKeyValueParser::LogLevel::WARN);
    } else {
        advance();
    }
    return ValveKeyValueParser::trim(m_buffer.substr(string_start, string_end - string_start));
}

ValveKeyValueFormat::TokenPair ValveKeyValueFormat::KVLexer::next_token() {
    char symbol = this->symbol();
    while (isspace(symbol) && symbol != '\n') {
        advance();
        symbol = this->symbol();
    }
    if (symbol == '\n') {
        advance();
        return {ValveKeyValueFormat::TokenTypes::NEWLINE, R"(\n)"sv};
    } else if (symbol == '/' && next_symbol() == '/') {
        advance();
        advance();
        read_simple_string("\n{"sv);
        return next_token();
        //        return {ValveKeyValueFormat::TokenTypes::COMMENT, read_simple_string("\n{"sv)};
    } else if (symbol == '{') {
        advance();
        return {ValveKeyValueFormat::TokenTypes::LBRACE, "{"};
    } else if (symbol == '}') {
        advance();
        return {ValveKeyValueFormat::TokenTypes::RBRACE, "}"};
    } else if (symbol == '[') {
        advance();
        return {ValveKeyValueFormat::TokenTypes::LBRACKET, "["};
    } else if (symbol == ']') {
        advance();
        return {ValveKeyValueFormat::TokenTypes::RBRACKET, "]"};
    } else if (symbol == '"' || symbol == '\'') {
        if (next_symbol() == '"' || next_symbol() == '\'') {
            advance();
            advance();
            return {ValveKeyValueFormat::TokenTypes::STRING, ""sv};
        }
        auto string = read_quoted_string();
        if (string.empty()) return next_token();;
        return {TokenTypes::STRING, string};
    } else if (isprint(symbol)) {
        return {TokenTypes::STRING, read_simple_string(" \t\n"sv)};
    }

    return {TokenTypes::END_OF_FILE, "EOF"sv};
}
