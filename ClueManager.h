#ifndef CLUEMANAGER_H
#define CLUEMANAGER_H

#include <string>
#include <vector>
#include <map>

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

    void initializeScenarios();
    void selectScenario();
    std::string replacePlaceholders(const std::string& templateStr);
};

#endif // CLUEMANAGER_H
