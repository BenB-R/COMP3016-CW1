#include "CharacterManager.h"
#include <fstream>
#include <sstream>

CharacterManager::CharacterManager() {
    // Constructor implementation (if necessary)
}

void CharacterManager::loadCharacterData(const std::string& filename) {
    parseCharacterFile(filename);
}

void CharacterManager::parseCharacterFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        CharacterInfo info;
        if (std::getline(iss, info.name, ',') &&
            std::getline(iss, info.time, ',') &&
            std::getline(iss, info.location, ',') &&
            std::getline(iss, info.dialogue, '\"')) {
            characterSchedules[info.name].push_back(info);
        }
    }
}

std::vector<CharacterInfo> CharacterManager::getCharacterSchedule(const std::string& name) {
    if (characterSchedules.find(name) != characterSchedules.end()) {
        return characterSchedules[name];
    }
    return std::vector<CharacterInfo>(); // Return empty vector if no schedule found for the character
}
