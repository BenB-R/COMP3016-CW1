#include "EndingManager.h"

EndingManager::EndingManager(ClueManager& clueManager) : clueManager(clueManager) {}

std::string EndingManager::getEndingMessage(bool playerWins) {
    if (playerWins) {
        return "In the dead of night at " + clueManager.getCurrentScenario().location +
            ",you spot " + clueManager.getCurrentScenario().murderer +
            " about to strike at " + clueManager.getCurrentScenario().victim +
            ". Your timely intervention and sharp detective skills prevent a gruesome murder. Well done, detective!";
    }
    else {
        return "In the eerie silence, you stumble upon a horrifying scene,\nA MURDER!\nThe reality hits you - but too late. Shadows move swiftly, and before you can react,\ndarkness engulfs you too. A chilling end to your investigation.";
    }
}
