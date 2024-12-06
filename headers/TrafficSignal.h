#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H
#include "CongestionHashTable.h"
#include<fstream>
#include<ncurses.h>




class TrafficSignal{

    struct Intersection{

        char name;
        bool isGreen;

        //time is represented in seconds
        int timeCounter; //variable to keep track of the time passed
        int totalTime; //original green time of the intersection
        int timeIncrement; //the value by which the green time was changes

        Intersection(char _name, int _totalTime);
        Intersection();

        void operator= (const Intersection &i);

    };

    CongestionHashTable congestion;

    Intersection *intersections;

    int getRandomValue(int min, int max) const;

    int hashTableSize;
    int congestionThreshold;
    int intersectionArrSize;

    public:

    TrafficSignal(int conThreshold = 5, int hashTSize = 727);

    ~TrafficSignal();

    void reduceCongestion(char name, int extraCars, bool turnGreenInstantly = false);

    void updateTime(); //update all the traffic lights time by 1 second

    void updateCongestion(int numOfRoads); //this will pick random roads and remove or add cars to it to simulate traffic flow

    void displayTraffic(WINDOW *win, int &line) const; //line is the variable storing current line in the window

};

#endif