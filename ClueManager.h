#ifndef CLUEMANAGER_H
#define CLUEMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

class ClueManager {
public:
    ClueManager();
    void loadCluesFromFile(const std::string& filename);
    std::string getDynamicClue(int index);

private:
    struct Scenario {
        std::string murderer;
        std::string victim;
        std::string location;
    };

    Scenario currentScenario;
    std::vector<Scenario> scenarios;
    std::vector<std::string> clueTemplates;
    std::unordered_map<int, std::string> storedClues; // Map to store generated clues

    void initializeScenarios();
    void selectScenario();
    void generateAllClues(); // Function to generate all clues
    std::string replacePlaceholders(const std::string& templateStr);
};

#endif // CLUEMANAGER_H
