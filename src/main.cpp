
#include "VKVParser/kv_lexer.hpp"
#include <VKVParser/kv_parser.hpp>
#include <string>

using namespace std::literals;

ValveKeyValueFormat::LoggerFunction* ValveKeyValueFormat::logger_function = [](const std::string& message, ValveKeyValueFormat::LogLevel severity) -> void { fprintf(stderr, "%s\n", message.c_str());fflush(stderr); };

int main(int argc, char** argv) {
    auto test_vmt = "\n"
                    "Shader //Test comment\n"
                    "{ //second comment m_line\n"
                    "    $key\"  \"value\n"
                    "    \"$key1\"  \"value1'\n"
                    "    \"$key2\"  \"value1\" //Comment   \n"
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
    auto parser = ValveKeyValueFormat::KVParser(test_vmt);
    parser.parse();
    auto root = parser.root()->as_branch();
    auto key4 = root->get("$key4");
    if (key4) printf("LOWCASE %s\n", key4->as_leaf()->value.c_str());
    key4 = root->get("$KEY4");
    if (key4) printf("UPCASE %s\n", key4->as_leaf()->value.c_str());
    auto keyNo = root->get("$key4123");
    if (keyNo) printf("UPCASE %s\n", key4->as_leaf()->value.c_str());
    key4->get_key();
    return 0;
}