#include "OnOffSensor.h"

OnOffSensor::OnOffSensor(bool pIsActive)
{

}
OnOffSensor::OnOffSensor(bool pActState, bool pLastState, bool pChangedFlag, bool pIsActive,  DateTime pLastSwitchTime)
{ 
    actState = pActState;
    lastState = pLastState;
    changedFlag = pChangedFlag;
    isActive = pIsActive;
    lastSwitchTime = pLastSwitchTime;
}

void OnOffSensor:: Feed(bool pNewState, DateTime pDate)
{
    if (pNewState != actState)
    {
        lastState = actState;
        actState = pNewState;
        changedFlag = true;
        lastSwitchTime = pDate;
    }
}

bool OnOffSensor::HasChangedState()
{
    if (isActive)
    {
        return changedFlag;   
    }
    else
    {
        return false;
    }
}

bool OnOffSensor::GetStateResetChangedFlag()
{

    changedFlag = false;
    return actState;
}

bool OnOffSensor::GetState()
{
    return actState;
}

void OnOffSensor::SetInactive()
{
    isActive = false;
}

void OnOffSensor::SetActive()
{
    isActive = true;
}