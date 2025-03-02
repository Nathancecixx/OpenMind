#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>


class Prompt {
public:
    std::string first;
    std::string second;

    Prompt(std::string first, std::string second):first(first), second(second){};
};

class PromptGenerator {
    std::vector<Prompt> m_prompts;
    std::string m_filePath;

public:
    PromptGenerator(std::string FilePath);

    bool LoadPrompts();

    Prompt GetRandomPrompt();

};