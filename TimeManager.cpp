// TimeManager.cpp

#include "TimeManager.h"

TimeManager::TimeManager() : currentTime(MORNING) {}

void TimeManager::advanceTime() {
    switch (currentTime) {
    case MORNING:
        currentTime = NOON;
        break;
    case NOON:
        currentTime = AFTERNOON;
        break;
    case AFTERNOON:
        currentTime = EVENING;
        break;
    case EVENING:
        currentTime = NIGHT;
        break;
    case NIGHT:
        currentTime = MORNING;
        break;
    }
    updateGameEnvironment(currentTime);
}

TimeManager::TimeOfDay TimeManager::getCurrentTime() const {
    return currentTime;
}

void TimeManager::updateGameEnvironment(TimeOfDay time) {
    // Update game visuals and mechanics based on the time of day
    // This can be expanded based on your game's specific requirements
}
