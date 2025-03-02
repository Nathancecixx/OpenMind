#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


class Prompt {
public:
    std::string first;
    std::string second;

    Prompt(std::string first, std::string second):first(first), second(second){};
};

class PromptGenerator {
    std::vector<Prompt> m_prompts;
    std::string m_filePath;

    PromptGenerator(std::string FilePath);

    bool LoadPrompts();

    Prompt GetRandomPrompt();

};