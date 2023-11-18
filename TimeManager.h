// TimeManager.h

#include <string>
#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

class TimeManager {
public:
    enum TimeOfDay { MORNING, NOON, AFTERNOON, EVENING, NIGHT };

    TimeManager();

    std::string getCurrentTimeAsString() const;
    void setDayTime(TimeOfDay newTime);

    void advanceTime();
    TimeOfDay getCurrentTime() const;

private:
    TimeOfDay currentTime;

    void updateGameEnvironment(TimeOfDay time);
};

#endif // TIMEMANAGER_H
