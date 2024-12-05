#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H
#include "CongestionHashTable.h"
#include<fstream>




class TrafficSignal{

    struct Intersection{

        char name;
        bool isGreen;
        int timeCounter; 
        int totalTime;
        int timeIncrement; 

        Intersection(char _name, int _totalTime);

    };

    CongestionHashTable congestion;

    Intersection *intersections;

    int getRandomValue(int min, int max);

    public:

    TrafficSignal();

    ~TrafficSignal();

    void displayTraffic();

    void reduceCongestion(char intersectioName);

    void update();



};

#endif