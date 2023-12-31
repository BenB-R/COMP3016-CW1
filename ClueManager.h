#ifndef CLUEMANAGER_H
#define CLUEMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

class ClueManager {
public:
    struct Scenario {
        std::string murderer;
        std::string victim;
        std::string location;
    };
    Scenario getCurrentScenario() const;
    ClueManager();
    void loadCluesFromFile(const std::string& filename);
    std::string getDynamicClue(int index, int& outWeight);
    int processAndRemoveMetadata(std::string& clueText);
	int totalClueWeight = 0;

private:


    int totalCoreSupplementaryWeight;  // Holds the total weight for Core and Supplementary clues

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
