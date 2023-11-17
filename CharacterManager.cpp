#include "CharacterManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

CharacterManager::CharacterManager() {

}

void CharacterManager::loadCharacterData(const std::string& filename) {
    parseCharacterFile(filename);
}


// Utility function to trim whitespace from both ends of a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == std::string::npos || last == std::string::npos) return str;
    return str.substr(first, (last - first + 1));
}

void CharacterManager::parseCharacterFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    std::cout << "Parsing character data from file: " << filename << std::endl;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        CharacterInfo info;
        if (std::getline(iss, info.name, ',') &&
            std::getline(iss, info.time, ',') &&
            std::getline(iss, info.location, ',')) {

            iss >> std::ws; // Eat up any leading whitespace before the quote
            char quote;
            iss >> quote; // Read the next character, which should be the opening quote

            if (quote == '\"') {
                std::getline(iss, info.dialogue, '\"'); // Get the dialogue up to the closing quote

                // Trim the time and location strings
                info.time = trim(info.time);
                info.location = trim(info.location);

                // Debug output for each character's parsed data
                std::cout << "Parsed character: " << info.name
                    << ", Time: " << info.time
                    << ", Location: " << info.location
                    << ", Dialogue: " << info.dialogue << std::endl;

                characterSchedules[info.name].push_back(info);
            }
            else {
                std::cerr << "Expected quote at start of dialogue, got: " << quote << std::endl;
            }
        }
        else {
            std::cerr << "Failed to parse line: " << line << std::endl;
        }
    }

    if (file.bad()) { // error handling
        std::cerr << "An I/O error occurred while reading the file." << std::endl;
    }
    else if (!file.eof()) {
        std::cerr << "The file did not end as expected." << std::endl;
    }
}


std::vector<CharacterInfo> CharacterManager::getCharactersAt(const std::string& timeOfDay, const std::string& location) {
    std::vector<CharacterInfo> characters;
    std::cout << "Looking for characters at time: '" << timeOfDay << "' and location: '" << location << "'" << std::endl;
    for (const auto& pair : characterSchedules) {
        for (const auto& info : pair.second) {
            std::cout << "Checking character: " << info.name << " at time: '" << info.time << "' and location: '" << info.location << "'" << std::endl;
            if (info.time == timeOfDay && info.location == location) {
                characters.push_back(info);
            }
        }
    }
    return characters;
}

std::vector<CharacterInfo> CharacterManager::getCharacterSchedule(const std::string& name) {
    if (characterSchedules.find(name) != characterSchedules.end()) {
        return characterSchedules[name];
    }
    return std::vector<CharacterInfo>(); // Return empty vector if no schedule found for the character
}
