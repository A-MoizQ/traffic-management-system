#include "../headers/TrafficSignal.h"

/*
TODO:
- Write a display function
- return the intersection time back to normal if congestion is resolved
*/

TrafficSignal::Intersection::Intersection(char _name, int _totalTime) : 

    name(_name) ,
    isGreen(false),
    timeCounter(0),
    totalTime(_totalTime),
    timeIncrement(0)

    {
        if(totalTime<0)
            totalTime = 1;

    }

TrafficSignal::Intersection::Intersection() :

    name(0),
    isGreen(false),
    timeCounter(0),
    totalTime(0),
    timeIncrement(0)

    {}


void TrafficSignal::Intersection::operator= (const Intersection &i) {

    name = i.name;
    isGreen = i.isGreen;
    timeCounter = i.timeCounter;
    totalTime = i.totalTime;
    timeIncrement = i.timeIncrement;

}


TrafficSignal::TrafficSignal(int conThreshold, int hashTSize) :


    congestion(conThreshold, hashTSize),
    hashTableSize(hashTSize),
    congestionThreshold(conThreshold),
    intersectionArrSize(128) //to store ascii from 0 - 127

 {

    //this is a kind of hash map which will store the intersection info
    //at the index equal to the ascii of the name of the intersection
    intersections = new Intersection[intersectionArrSize];

    for(int i = 0;i<intersectionArrSize;i++){
        Intersection emptyIntersection; //make an empty intersection
        intersections[i] = emptyIntersection; //this indicates that the index is empty
    }




    //add the starting data from the file to the congestion hash table
    std::fstream fileHandler("road_network.csv", std::ios::in);
    if(!fileHandler){
        erase();
        mvprintw(0,0,"road_network.csv not found!");
        refresh();
        return;
    }
    std::string data;
    getline(fileHandler,data); //to skip the headers

    while(!fileHandler.eof()){

        //get intersection1 name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char intersection1Name = data[0];

        //get intersection1 name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char intersection2Name = data[0];

        //get number of vehicles
        getline(fileHandler,data,'\n');
        if(data.empty()) continue;
        //convert to int
        int numOfCars = stoi(data);

        //add in the congestion table
        congestion.insert(intersection1Name, intersection2Name, numOfCars);

    }
    fileHandler.close();


    //now read the traffic signal data
    std::fstream signalFileHandler("traffic_signals.csv", std::ios::in);
    if(!signalFileHandler){
        erase();
        mvprintw(0,0,"traffic_signals.csv not found!");
        refresh();
        return;
    }
    getline(signalFileHandler,data); //to skip the headers

    while(!signalFileHandler.eof()){

        //get intersection name
        getline(signalFileHandler,data,',');
        if(data.empty()) continue;
        char intersectionName = data[0];
        
        //convert the name to ascii to do indexing in the array
        int intrASCII = static_cast<int>(intersectionName);

        //get green time
        getline(signalFileHandler,data,',');
        if(data.empty()) continue;
        int greenTime = static_cast<int>(data[0]);

        if( (intersections[intrASCII]).name == 0 ){ //if the index is empty

            Intersection i(intersectionName, greenTime); //make an object for the intersection
            intersections[intrASCII] = i; //add to the array

        }

    }
    signalFileHandler.close();

}

TrafficSignal::~TrafficSignal(){

    delete[] intersections;

}

int TrafficSignal::getRandomValue(int min, int max) const {

    return min + (rand() % (max - min + 1));

}

void TrafficSignal::reduceCongestion(char name, int extraCars, bool turnGreenInstantly){

    if(name >=intersectionArrSize || name <0 ) //invalid name
        return;


    Intersection i = intersections[ static_cast<int>(name) ]; //get the intersection object

    if(extraCars <=0 ){ //this means there is no congestion
        //revert to original state
        i.timeIncrement = 0;
        return;
    }


    i.timeIncrement = extraCars * 2;

    if(extraCars >= 2*congestionThreshold) //if the road is very congested then turn the signal green instanltly
        turnGreenInstantly = true;

    if(turnGreenInstantly){
        i.isGreen = true;
        i.timeCounter = i.totalTime + i.timeIncrement;
        return;
    }


}

void TrafficSignal::updateTime(){

    for (int i = 0 ; i < intersectionArrSize ; i++) { //loop through all intersections

        Intersection inter = intersections[i];

        if( inter.name != 0 ){ //if the index is not empty

            ((intersections[i]).timeCounter) -= 1; //reduce the elpased time

            if( intersections[i].timeCounter == 0 ) //if the green time is up, turn the light red
                intersections[i].isGreen = false;

            //if red time (which will be negative) is up i.e redtime == timeIncrement-totalTime 
            //timeIncrement-totalTime is used instead of just totalTime to reduce the red time when road is congested
            else if( intersections[i].timeCounter <= intersections[i].timeIncrement - intersections[i].totalTime  ){ 
                intersections[i].isGreen = true;
                intersections[i].timeCounter = intersections[i].totalTime + intersections[i].timeIncrement; //reset the time
            }

        }

    }

}

void TrafficSignal::updateCongestion(int numOfRoads) {

    if(numOfRoads<=0 || numOfRoads>=hashTableSize) //input validation
        return;

    for  ( int i = 0 ; i < numOfRoads ; i++ ) {

        int randomIndx = getRandomValue(0, hashTableSize);
        IntersectionPair road = congestion.getInterLinearProbing(randomIndx); //get a random road through linear probing

        bool add = getRandomValue(0,1); //randomly add or remove cars
        int numOfCars = congestion.getNumOfCars ( road.intersection1, road.intersection2 ) ;
        int change = getRandomValue(0, numOfCars); //this will be added or subtracted from numOfCars on road

        if(add)
            numOfCars += change;
        else{
            //suppose cars are going from intersection1 to intersection2
            //only remove the cars from the road if the signal is green bcz cars can only move when signal is green duh
            if( intersections[static_cast<int>(road.intersection2)].isGreen )
                numOfCars -= change;
        }

        //update the value in the congestion table
        congestion.updateRoad( road.intersection1, road.intersection2, numOfCars ); 

        int extraCars = numOfCars - congestionThreshold;
        reduceCongestion(road.intersection2, extraCars);
        
    }

}

void TrafficSignal::displayTraffic(WINDOW *win, int &line) const {

    bool isEmpty = true;

    //first rpint intersections status

    mvwprintw(win, line++, 1, "=== Traffic Signal Status ===");

    //supposing the intersection names are from A - Z
    for (char indx = 'A'; indx <= 'Z' ; indx++) {

        Intersection i = intersections[static_cast<int>(indx)];

        if(i.name == 0) //empty index
            continue;
        else
            isEmpty = false;

        if ( i.isGreen )
            mvwprintw(win,line++, 1, (i.name + " Status : Green , Time Till Red : " + std::to_string(i.timeCounter) + "s").c_str());
        else {
            mvwprintw(win,line++, 1, (i.name + " Status : Red , Time Till Green : " + std::to_string(i.totalTime - i.timeCounter - i.timeIncrement)).c_str());
            isEmpty = false;
        }
    }

    //then print the number of cars on each road
    mvwprintw(win, line++, 1, "=== Roads Congestion Status ===");

    //indx indexes will start at the ASCII of A in this hash map
    for (int indx = 'A'; indx < hashTableSize ; ) {

        //if the loop circles back to the starting index
        if(indx < 'A')
            break; 

        IntersectionPair road = congestion.getInterLinearProbing(indx); //get a pair through linear probing
        indx = congestion.hash(road) + 1; //get the index of the intersection we got through linear probing

        if(road.intersection1 == 0 || road.intersection2) //empty intersection
            continue;
        else
            isEmpty = false;


        mvwprintw(win, line++, 1, ( "Road from " + std::to_string(road.intersection1) + " to " + std::to_string(road.intersection2) + " , Number of cars : " + std::to_string(congestion.getNumOfCars(road.intersection1, road.intersection2)) ).c_str());
        
    }

    if (isEmpty) {
        mvwprintw(win, line++, 1, "No road congestion and traffic signal data found.");
    }

    wrefresh(win); // Refresh ncurses window

}