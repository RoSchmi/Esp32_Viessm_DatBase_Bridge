#include <Arduino.h>
#include <Datetime.h>

#ifndef _ON_OFF_SENSOR_H_
#define _ON_OFF_SENSOR_H_

class OnOffSensor
{
public:
    OnOffSensor(bool pIsActive);
    OnOffSensor(bool pactState, bool pLastState, bool pChangedFlag, bool pIsActive,  DateTime pLastSwitchTime);
    
    void begin(TimeSpan interval);

    void SetInactive();
    void SetActive();
    void Feed(bool newState, DateTime actDate);
    bool HasChangedState();
    //bool hasToggled(DateTime actUtcTime);
    bool GetState();
    bool GetStateResetChangedFlag();

private:
    bool actState = false;
    bool lastState = false;
    bool isActive = false;
    bool changedFlag = false;
    DateTime  lastSwitchTime = DateTime();
};

#endif