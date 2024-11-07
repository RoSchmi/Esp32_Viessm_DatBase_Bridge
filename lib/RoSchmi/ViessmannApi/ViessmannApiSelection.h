#include <Arduino.h>

#ifndef _VIESSMANNAPISELECTION_H_
#define _VIESSMANNAPISELECTION_H_

#define FEATUREVALUELENGTH 12
#define FEATURENAMELENGTH 60
#define FEATURESTAMPLENGTH 30

class ViessmannApiSelection
{
    public:  
    int  valueLength = FEATUREVALUELENGTH;
    int nameLenght = FEATURENAMELENGTH;
    int stampLength = FEATURESTAMPLENGTH;
    
    typedef struct
    {     
        char name[FEATURENAMELENGTH] = {0};
        char timestamp[FEATURESTAMPLENGTH] = {0};     
        char value[FEATUREVALUELENGTH] = {0};       
    }Feature;
    
    ViessmannApiSelection();
    ~ViessmannApiSelection();

    Feature _3_temperature_main;
    Feature _5_Boiler_temperature;  
};

#endif