#include "../headers/TrafficSignal"

/*
TODO:
- Write a display function
- return the intersection time back to normal if congestion is resolved
*/

TrafficSignal::Intersection::Intersection(char _name, int _totalTime) : 

    name(_name) ,
    totalTime(_totalTime),
    isGreen(false),
    timeCounter(0),
    timeIncrement(0)

    {
        if(totalTime<0)
            totalTime = 1;

    }

TrafficSignal::Intersection::Intersection() :

    name('NUL'),
    isGreen(false),
    timeCounter(0),
    totalTime(0),
    timeIncrement(0)

    {}


TrafficSignal::Intersection::operator= (const Intersection &i) {

    name = i.name;
    isGreen = i.isGreen;
    timeCounter = i.timCounter;
    totalTime = i.totalTime;
    timeIncrement = i.timeIncrement;

}


TrafficSignal::TrafficSignal(int conThreshold, int hashTSize) :

    congestionThreshold(conThreshold),
    hashTableSize(hashTSize),
    //pass the congestion threshold and hashtable size
    congestion(congestionThreshold, hashTSize),
    intersectionArrSize(128) //to store ascii from 0 - 127

 {

    //this is a kind of hash map which will store the intersection info
    //at the index equal to the ascii of the name of the intersection
    intersections = new char[intersectionArrSize];

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
    std::fstream fileHandler("traffic_signals.csv", std::ios::in);
    if(!fileHandler){
        erase();
        mvprintw(0,0,"traffic_signals.csv not found!");
        refresh();
        return;
    }
    std::string data;
    getline(fileHandler,data); //to skip the headers

    while(!fileHandler.eof()){

        //get intersection name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char intersectionName = data[0];
        
        //convert the name to ascii to do indexing in the array
        int intrASCII = static_cast<int>(intersectionName)

        //get green time
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        int greenTime = static_cast<int>(data[0]);

        if( intersections[intrASCII] == '0' ){ //if the index is empty

            intersection i(intersectionName, greenTime);

        }

    }
    fileHandler.close();

}

TrafficSignal::~TrafficSignal(){

    delete[] intersections;

}

TrafficSignal::getRandomValue(int min, int max) const {

    return min + (rand() % (max - min + 1));

}

void reduceCongestion(char name, int extraCars, bool turnGreenInstantly = false){

    if(name >=intersectionArrSize || name <0 ) //invalid name
        return;


    Intersection i = intersections[ static_cast<int>name ]; //get the intersection object

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

void updateTime(){

    for (int i = 0 ; i < intersectionArrSize ; i++) { //loop through all intersections

        Intersection inter = intersections[i];

        if( inter.name != 'NUL' ){ //if the index is not empty

            ((intersections[i]).timeCounter) -= 1; //reduce the elpased time

            if( intersections[i].timeCounter == 0 ) //if the green time is up, turn the light red
                intersections[i],isGreen = false;

            //if red time (which will be negative) is up i.e redtime == timeIncrement-totalTime 
            //timeIncrement-totalTime is used instead of just totalTime to reduce the red time when road is congested
            else if( intersections[i].timeCounter <= intersections[i].timeIncrement - intersections[i].totalTime  ){ 
                intersections[i].isGreen = true;
                intersections[i].timeCounter = totalTime + timeIncrement; //reset the time
            }

        }

    }

}

void updateCongestion(int numOfRoads) {

    if(numOfRoads<=0 || numOfRoads>=hashTableSize) //input validation
        return;

    for  ( int i = 0 ; i < numOfRoads ; i++ ) {

        int randomIndx = getRandomValue(0, hashTableSize);
        IntersectionPair road = getIntersectionAfterIndx(randomIndx); //get a random road through linear probing

        bool add = getRandomValue(0,1); //randomly add or remove cars
        int numOfCars = getNumOfCars ( road.intersection1, road.intersection2 ) ;
        int change = getRandomValue(0, numOfCars); //this will be added or subtracted from numOfCars on road

        if(add)
            numOfCars += change;
        else{
            //suppose cars are going from intersection1 to intersection2
            //only remove the cars from the road if the signal is green bcz cars can only move when signal is green duh
            if( intersections[intersection2].isGreen )
                numOfCars -= change;
        }

        //update the value in the congestion table
        congestion.updateRoad( road.intersection1, road.intersection2, numOfCars ); 

        int extraCars = numOfCars - congestionThreshold;
        reduceCongestion(road.intersection2, extraCars);
        
    }

}

void displayTraffic(WINDOW *win, int &line) const {



}