
#include "vmt_lexer.hpp"
#include <shared.hpp>
#include <string>
#include <vmt_parser.hpp>

using namespace std::literals;

LoggerFunction* logger_function = [](const std::string& message, LogLevel severity) -> void { fprintf(stderr, "%s\n", message.c_str());fflush(stderr); };

int main(int argc, char** argv) {
    auto test_vmt = "\n"
                    "Shader //Test comment\n"
                    "{ //second comment m_line\n"
                    "    $key\"  \"value\n"
                    "    \"$key1\"  \"value1'\n"
                    "    \"$key2\"  \"value1\"\n"
                    "    \"$key3\"  \"value2`\n"
                    "    \"$key4\"  \"value3\"  [!PS3]\n"
                    "    \"$key4\"  \"value5342\"  [PS3]\n"
                    "    \"$key5\"  \"[0.0 1.0 1.0]\"\n"
                    "    \"$key5\"  \"{255 255 255 4100}\"\n"
                    "    \"$key6\"  models/tests/basic_vertexlitgeneric\n"
                    "    \n"
                    "    test \"<>!=asd./\"\n"
                    "    \n"
                    "    $envtint  .3 .3 .3\n"
                    "    \n"
                    "    $basetexturetransform  \"center .5 .5 scale 1 1 rotate 16 translate 15 8\"\n"
                    "    \">=DX80\"{\n"
                    "        key1 valie1\n"
                    "    \n"
                    "    }\n"
                    "    Proxies{\n"
                    "        Test\n"
                    "        {\n"
                    "           key1 valie1\n"
                    "        }\n"
                    "        test   test\n"
                    "    \n"
                    "    }\n"
                    "}"sv;
    //    auto lexer = ValveKeyValueFormat::VmtLexer(test_vmt);
    //    ValveKeyValueFormat::TokenPair token;
    //    do {
    //        token = lexer.next_token();
    //
    //        printf("TokenID %i token value: \"%s\"\n", token.first, std::string(token.second).c_str());
    //        fflush(stdout);
    //    } while (token.first != ValveKeyValueFormat::TokenTypes::END_OF_FILE);
    auto parser = ValveKeyValueFormat::VmtParser(test_vmt);
    parser.parse();
    auto root = parser.root()->as_branch();
    auto key4 = root->get("$key4");
    auto keyNo = root->get("$key4123");
    key4->get_key();
    return 0;
}