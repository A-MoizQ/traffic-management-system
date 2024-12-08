#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H
#include "CongestionHashTable.h"
#include<fstream>
#include<ncurses.h>
#include<string>




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

    CongestionHashTable& congestion;

    int intersectionArrSize;

    Intersection *intersections;

    int getRandomValue(int min, int max) const;

    public:

    TrafficSignal(CongestionHashTable& _congestion);

    ~TrafficSignal();

    void readTrafficSignalFile (std::string filename , WINDOW *win ) ;

    void reduceCongestion(char name, int extraCars, bool turnGreenInstantly = false);

    void updateTime(); //update all the traffic lights time by 1 second

    void updateCongestion(int numOfRoads); //this will pick random roads and remove or add cars to it to simulate traffic flow

    void displayTraffic(WINDOW *win, int &line, int col) const; //line is the variable storing current line in the window

    void displaySignals(WINDOW *win, int &line,int col, bool refreshWindow = true) const;

    float getRedTimeLeft( char intersectionName ) const;

};

#endif