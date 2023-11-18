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

void TimeManager::setDayTime(TimeOfDay newTime) {
    currentTime = newTime;
    updateGameEnvironment(currentTime);
}

std::string TimeManager::getCurrentTimeAsString() const {
    switch (currentTime) {
    case MORNING:
        return "Morning";
    case NOON:
        return "Noon";
    case AFTERNOON:
        return "Afternoon";
    case EVENING:
        return "Evening";
    case NIGHT:
        return "Night";
    default:
        return "Unknown";
    }
}

TimeManager::TimeOfDay TimeManager::getCurrentTime() const {
    return currentTime;
}

void TimeManager::updateGameEnvironment(TimeOfDay time) {
    // Update game visuals and mechanics based on the time of day
}