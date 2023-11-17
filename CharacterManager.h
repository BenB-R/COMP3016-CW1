#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include <string>
#include <vector>
#include <map>

struct CharacterInfo {
    std::string name;
    std::string time;
    std::string location;
    std::string dialogue;
};

class CharacterManager {
public:
    CharacterManager();
    void loadCharacterData(const std::string& filename);
    std::vector<CharacterInfo> getCharacterSchedule(const std::string& name);
    std::vector<CharacterInfo> getCharactersAt(const std::string& timeOfDay, const std::string& location);
private:
    std::map<std::string, std::vector<CharacterInfo>> characterSchedules; // Key: Character Name
    void parseCharacterFile(const std::string& filename);
};

#endif // CHARACTER_MANAGER_H
