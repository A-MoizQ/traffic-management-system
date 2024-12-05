#include "../headers/TrafficSignal"


/*
TODO: 
- make intersection default constructor and = 
- init intersections with empty intersections instead of 'NUL' in TrafficSignal()
- Write a display function
- add data members for size of arrays / hashMaps
- return the intersection time back to normal if congestion is resolved
*/

TrafficSignal::Intersection::Intersection(char _name, int _totalTime) : 

    name(_name) ,
    totalTime(_totalTime),
    isGreen(false),
    timeCounter(0),
    timeIncrement(0)

    {}


TrafficSignal::TrafficSignal() :

    //pass the congestion threshold and hashtable size
    congestion(5, 727)

 {

    //this is a kind of hash map which will store the intersection info
    //at the index equal to the ascii of the name of the intersection
    intersections = new char[127];

    for(int i = 0;i<127;i++){
        intersections[i] = 'NUL'; //this indicates that the index is empty
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

TrafficSignal::getRandomValue(int min, int max) {

    return min + (rand() % (max - min + 1));

}

void reduceCongestion(char name, int extraCars, bool turnGreenInstantly = false){

    Intersection i = intersections[ static_cast<int>name ]; //get the intersection object

    i.timeIncrement = extraCars * 2;

    if(turnGreenInstantly){
        i.isGreen = true;
        i.timeCounter = i.totalTime + i.timeIncrement;
        return;
    }

    //add the incremented time only when the light is green.
    //if the light is not green i.e red then do this:
    //when the timCounter is <= timeIncrement then turn the light green
    //dont wait till it goes to 0
    //this way we will decrease the red time as well


}

void updateTime(){

    for (int i = 0 ; i < 127 ; i++) { //loop through all intersections

        Intersection inter = intersections[i];

        if( inter.name != 'NUL' ){ //if the intersection is not empty

            ((intersections[i]).timeCounter) -= 1;

            if( intersections[i].timeCounter == 0 ) //if the green time is up, turn the light red
                intersections[i],isGreen = false;

            else if( intersections[i].timeCounter <= intersections[i].totalTime * -1 ){ //if red time (which will be negative) is up i.r redtime == -totalTime 
                intersections[i].isGreen = true;
                intersections[i].timeCounter = totalTime; //reset the time
            }

        }

    }

}

void update(int numOfRoads) {

    updateTime();

    if(numOfRoads<=0)
        return;

    for  ( int i = 0 ; i < numOfRoads ; i++ ) {

        int randomIndx = getRandomValue(0, 726);
        IntersectionPair road = getIntersectionAfterIndx(randomIndx); //get a random road

        bool add = getRandomValue(0,1); //randomly add or remove cars
        int numOfCars = getNumOfCars ( road.intersection1, road.intersection2 ) ;
        int change = getRandomValue(0, numOfCars); //this will be added or subtracted from numOfCars on road
        if(add)
            numOfCars += change;
        else{
            //suppose cars are going from intersection1 to intersection2
            //only remove the cars from the road if the intersection2 is green
            if( intersections[intersection2].isGreen )
                numOfCars -= change;
        }

        //update the value in the congestion table
        congestion.updateRoad( road.intersection1, road.intersection2, numOfCars ); 

        //if the road is congested
        if(numOfCars > congestionThreshold) {

            int extraCars = numOfCars - congestionThreshold;

            if(numOfCars >= 2*congestionThreshold) //if the road is very congested then turn the signal green instanltly
                reduceCongestion( road.intersection2,extraCars, true  );
            else
                reduceCongestion( road.intersection2,extraCars, false  );

        }

    }

}

