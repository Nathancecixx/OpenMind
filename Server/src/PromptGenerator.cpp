
#include "PromptGenerator.h"

PromptGenerator::PromptGenerator(std::string FilePath){
    m_filePath = FilePath;
}

bool PromptGenerator::LoadPrompts(){
    std::ifstream file(m_filePath);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) { 
        std::string first, second;
        std::istringstream iss(line);
        std::getline( iss, first, ':');
        std::getline( iss, second);

        m_prompts.push_back(Prompt(first, second));
    }

    file.close();
}

Prompt GetRandomPrompt(){

}