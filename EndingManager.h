#ifndef ENDING_MANAGER_H
#define ENDING_MANAGER_H

#include <string>
#include "ClueManager.h" // Include this to access the scenario details

class EndingManager {
public:
    EndingManager(ClueManager& clueManager);
    std::string getEndingMessage(bool playerWins);

private:
    ClueManager& clueManager;
};

#endif // ENDING_MANAGER_H
