#include "ClueManager.h"
#include <fstream>
#include <random>
#include <iostream>
#include <sstream>

ClueManager::ClueManager() : totalCoreSupplementaryWeight(0) {
    initializeScenarios();
    selectScenario();
    loadCluesFromFile("ClueInfo.txt");
    generateAllClues();
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, scenarios.size() - 1);
    currentScenario = scenarios[distr(gen)];
    std::cout << "Selected scenario: Murderer=" << currentScenario.murderer
        << ", Victim=" << currentScenario.victim
        << ", Location=" << currentScenario.location << std::endl;
}

ClueManager::Scenario ClueManager::getCurrentScenario() const {
    return currentScenario;
}

void ClueManager::loadCluesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        clueTemplates.push_back(line);
        std::cout << "Loaded clue template: " << line << std::endl;
    }
}

void ClueManager::generateAllClues() {
    for (int i = 0; i < clueTemplates.size(); ++i) {
        std::string clue = replacePlaceholders(clueTemplates[i]);
        storedClues[i] = clue; // Store generated clue
        std::cout << "Generated clue for index " << i << ": " << clue << std::endl;
    }
}

std::string ClueManager::getDynamicClue(int index, int& outWeight) {
    auto it = storedClues.find(index);
    if (it != storedClues.end()) {
        std::string clueText = it->second;
        outWeight = processAndRemoveMetadata(clueText); // Extract and process metadata, and get the weight
        return clueText;
    }
    // Handle the case where the clue is not found
    outWeight = 0; // If there's no clue, set weight to 0
    return "Invalid clue index";
}


int ClueManager::processAndRemoveMetadata(std::string& clueText) {
    // Initialize metadata components
    std::string clueType;
    int weight = 0;

    // Split the string into parts based on commas
    std::vector<std::string> parts;
    std::stringstream ss(clueText);
    std::string part;
    while (std::getline(ss, part, ',')) {
        parts.push_back(part);
    }

    // Check if the clue format is correct (should be at least 6 parts)
    if (parts.size() >= 6) {
        // Extract the weight, which should be the second to last part
        try {
            weight = std::stoi(parts[parts.size() - 5]);
        }
        catch (const std::invalid_argument& ia) {
            std::cerr << "Invalid weight value: " << parts[parts.size() - 5] << '\n';
            return 0; // Return an error value, such as 0
        }

        // The clue type should be the last part
        clueType = parts.back();

        // Erase metadata from the clue text
        size_t pos = clueText.rfind(parts[parts.size() - 5]);
        if (pos != std::string::npos) {
            clueText.erase(pos - 1); // -1 to remove the comma before weight
        }
    }
    else {
        std::cerr << "Clue text format is incorrect." << std::endl;
        return 0; // Return an error value, such as 0
    }
    return weight;
}




std::string ClueManager::replacePlaceholders(const std::string& templateStr) {
    std::string result = templateStr;

    std::vector<std::string> allLocations = { "Gym", "Police Station", "Shop", "Cafe", "Pond", "Library", "Alex's House" };
    std::random_device rd;
    std::mt19937 gen(rd());

    // Replace [Murderer]
    size_t pos = result.find("[Murderer]");
    while (pos != std::string::npos) {
        result.replace(pos, 10, currentScenario.murderer);
        pos = result.find("[Murderer]", pos + currentScenario.murderer.length());
        std::cout << "After replacing [Murderer]: " << result << std::endl;
    }

    // Replace [Victim]
    pos = result.find("[Victim]");
    while (pos != std::string::npos) {
        result.replace(pos, 8, currentScenario.victim);
        pos = result.find("[Victim]", pos + currentScenario.victim.length());
        std::cout << "After replacing [Victim]: " << result << std::endl;
    }

    // Replace [Location]
    pos = result.find("[Location]");
    while (pos != std::string::npos) {
        std::string replacement = currentScenario.location;

        // Check if the clue is a red herring
        if (result.find("Red Herring") != std::string::npos) {
            std::uniform_int_distribution<> distr(0, allLocations.size() - 1);
            replacement = allLocations[distr(gen)]; // Choose a random location
        }

        result.replace(pos, 10, replacement);
        pos = result.find("[Location]", pos + replacement.length());
        std::cout << "After replacing [Location]: " << result << std::endl;
    }

    std::cout << "Final clue after replacements: " << result << std::endl;
    return result;
}