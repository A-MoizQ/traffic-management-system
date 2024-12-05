#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H
#include "CongestionHashTable.h"
#include<fstream>




class TrafficSignal{

    struct Intersection{

        char name;
        bool isGreen;
        int timeCounter; //variable to keep track of the time passed
        int totalTime; //original green time of the intersection
        int timeIncrement; //the value by which the green time was changes

        Intersection(char _name, int _totalTime);
        Intersection();

        void operator= (const Intersection &i);

    };

    CongestionHashTable congestion;

    Intersection *intersections;

    int getRandomValue(int min, int max);

    public:

    TrafficSignal();

    ~TrafficSignal();

    void displayTraffic();

    void reduceCongestion(char name, int extraCars, bool turnGreenInstantly = false);

    void update(int numOfRoads); //this will pick random roads and remove or add cars to it to simulate traffic flow



};

#endif