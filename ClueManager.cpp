#include "ClueManager.h"
#include <algorithm>
#include <fstream>
#include <random>

ClueManager::ClueManager() {
    initializeScenarios();
    selectScenario();
}

void ClueManager::initializeScenarios() {
    scenarios.push_back({ "Kacper", "Alex", "Gym" });
    scenarios.push_back({ "Ken", "Jack", "Police Station" });
    scenarios.push_back({ "George", "Alex", "Shop" });
    scenarios.push_back({ "Jack", "Dom", "Cafe" });
    scenarios.push_back({ "Dom", "Will", "Pond" });
    scenarios.push_back({ "Will", "George", "Library" });
    scenarios.push_back({ "Alex", "Kacper", "Alex's House" });
}

void ClueManager::selectScenario() {
    // Randomly select a scenario
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, scenarios.size() - 1);
    currentScenario = scenarios[distr(gen)];
}

void ClueManager::loadCluesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        clueTemplates.push_back(line);
    }
}

std::string ClueManager::getDynamicClue(int index) {
    if (index >= 0 && index < clueTemplates.size()) {
        return replacePlaceholders(clueTemplates[index]);
    }
    return "Invalid clue index";
}

std::string ClueManager::replacePlaceholders(const std::string& templateStr) {
    std::string result = templateStr;

    // Replace [Murderer]
    size_t pos = result.find("[Murderer]");
    while (pos != std::string::npos) {
        result.replace(pos, 10, currentScenario.murderer);
        pos = result.find("[Murderer]", pos + currentScenario.murderer.length());
    }

    // Replace [Victim]
    pos = result.find("[Victim]");
    while (pos != std::string::npos) {
        result.replace(pos, 8, currentScenario.victim);
        pos = result.find("[Victim]", pos + currentScenario.victim.length());
    }

    // Replace [Location]
    pos = result.find("[Location]");
    while (pos != std::string::npos) {
        result.replace(pos, 10, currentScenario.location);
        pos = result.find("[Location]", pos + currentScenario.location.length());
    }

    return result;
}



