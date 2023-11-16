// TimeManager.h

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

class TimeManager {
public:
    enum TimeOfDay { MORNING, NOON, EVENING, NIGHT };

    TimeManager();

    void advanceTime();
    TimeOfDay getCurrentTime() const;

private:
    TimeOfDay currentTime;

    void updateGameEnvironment(TimeOfDay time);
};

#endif // TIMEMANAGER_H
