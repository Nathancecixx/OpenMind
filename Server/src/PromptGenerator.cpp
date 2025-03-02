
#include "PromptGenerator.h"

PromptGenerator::PromptGenerator(std::string FilePath){
    m_filePath = FilePath;
}

bool PromptGenerator::LoadPrompts(){
    std::ifstream file(m_filePath);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
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
    return true;
}

Prompt PromptGenerator::GetRandomPrompt(){
    // If there are no prompts loaded, return a default or "empty" prompt.
    if (m_prompts.empty()) {
        return Prompt("", "");
    }

    // Set up a random engine and distribution (thread-safe approach).
    static std::random_device rd;  
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, m_prompts.size() - 1);

    // Pick a random prompt from the vector
    return m_prompts[dist(gen)];
}