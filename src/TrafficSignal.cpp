#include "../headers/TrafficSignal.h"
#include "../headers/Graph.h"
/*
TODO:
- return the intersection time back to normal if congestion is resolved
-for this we will need to get name of neighbours of a node, return it in string form e.g if neighbours are A,B,C return "ABC"
-then check the congestion levels on all roads the intersection connects to. if none are congested then return the time back to original
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


TrafficSignal::TrafficSignal(CongestionHashTable& _congestion) :

    congestion(_congestion),
    intersectionArrSize(128) //to store ascii from 0 - 127

{

    //this is a kind of hash map which will store the intersection info
    //at the index equal to the ascii of the name of the intersection
    intersections = new Intersection[intersectionArrSize];

    for(int i = 0;i<intersectionArrSize;i++){
        Intersection emptyIntersection; //make an empty intersection
        intersections[i] = emptyIntersection; //this indicates that the index is empty
    }

}

void TrafficSignal::readTrafficSignalFile(std::string filename, WINDOW *win) {

    std::string data;
    std::fstream signalFileHandler(filename, std::ios::in);
    if(!signalFileHandler){
        erase();
        mvwprintw(win, 0,0, (filename + " not found!").c_str() );
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
        getline(signalFileHandler,data, '\n');
        if(data.empty()) continue;

        int greenTime = std::stoi(data.substr(0,2));

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

    srand(time(0));
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

    if(extraCars >= 2*congestion.getConThreshold()) //if the road is very congested then turn the signal green instanltly
        turnGreenInstantly = true;

    if(turnGreenInstantly){
        i.isGreen = true;
        i.timeCounter = i.totalTime + i.timeIncrement;
        return;
    }


}

void TrafficSignal::updateTime(Graph &g){

    for (int i = 0 ; i < intersectionArrSize ; i++) { //loop through all intersections

        if( intersections[i].name != 0 ){ //if the index is not empty

            ((intersections[i]).timeCounter) -= 1; //reduce the elpased time

            if( intersections[i].timeCounter == 0 ) //if the green time is up, turn the light red
                intersections[i].isGreen = false;

            //if red time (which will be negative) is up i.e redtime == timeIncrement-totalTime 
            //timeIncrement-totalTime is used instead of just totalTime to reduce the red time when road is congested
            else if( intersections[i].timeCounter <= intersections[i].timeIncrement - intersections[i].totalTime  ){ 
                intersections[i].isGreen = true;
                intersections[i].timeCounter = intersections[i].totalTime + intersections[i].timeIncrement; //reset the time
            }

            //get the neighbours
            std::string neighbours = g.getAdjacencyList(intersections[i].name); //get the neightbours

            bool restoreTime = true;

            //go through each road that intersection is connected to
            for(char intersection : neighbours) {

                //get num of cars between the intersections
                int numOfCars = congestion.getNumOfCars(intersection, intersections[i].name);

                //if any of the roads is congested then dont restore time
                if(numOfCars >= congestion.getConThreshold()){
                    restoreTime = false;
                    break;
                }

            }

            if(restoreTime){

                intersections[i].timeIncrement = 0; //restore to default value

            }

        }

    }

}

void TrafficSignal::updateCongestion(int numOfRoads) {

    if(numOfRoads<=0 || numOfRoads>=congestion.getSize()) //input validation
        return;

    for  ( int i = 0 ; i < numOfRoads ; i++ ) {

        int randomIndx = getRandomValue(0, congestion.getSize()-1);
        Road road = congestion.getRoadLinearProbing(randomIndx); //get a random road through linear probing

        if(road.intersection1 == 0 || road.intersection2 == 0)
            continue;

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

        int extraCars = numOfCars - congestion.getConThreshold();
        reduceCongestion(road.intersection2, extraCars);
        
    }

}

void TrafficSignal::displayTraffic(WINDOW *win, int &line, int col) const {

    mvwprintw(win, line++, col, "=== Roads Congestion Status ===");

    //indexes will start at the ASCII of A in this hash map
    for (int indx = 'A', roadsFound = 0; indx < congestion.getSize() ;) {

        if( roadsFound >= congestion.getNumOfRoads() ) //if we have accessed all roads then break the loop
            break;

        //if the loop circles back to the starting index
        if(indx < 'A')
            break; 

        Road road = congestion.getRoadLinearProbing(indx); //get a pair through linear probing
        indx = congestion.hash(road) + 1; //get the index of the intersection we got through linear probing

        if(road.intersection1 == 0 || road.intersection2 == 0) //empty intersection
            continue;

        roadsFound++;



        mvwprintw(win, line++, col, ( "Road from " + std::string(1,road.intersection1) + " to " + std::string(1,road.intersection2) + " , Number of cars : " + std::to_string(congestion.getNumOfCars(road.intersection1, road.intersection2)) ).c_str());
        
    }

}

void TrafficSignal::displaySignals(WINDOW *win, int &line,int col, bool refreshWindow) const {


    mvwprintw(win, line++, col, "=== Traffic Signal Status ===");

    //supposing the intersection names are from A - Z
    for (char indx = 'A'; indx <= 'Z' ; indx++) {

        Intersection i = intersections[static_cast<int>(indx)];

        if(i.name == 0) //empty index
            continue;

        std::string strName = "";
        strName += i.name; 

        if ( i.isGreen )
            mvwprintw(win,line++, col, ("Name: " + strName + " Status : Green , Time Till Red : " + std::to_string(i.timeCounter) + "s").c_str());
        else {
            mvwprintw(win,line++, col, ("Name: " + strName + " Status : Red , Time Till Green : " + std::to_string(i.totalTime + i.timeCounter - i.timeIncrement) + "s").c_str());
        }
    }

}

float TrafficSignal::getRedTimeLeft( char intersectionName ) const {

    int indx = static_cast<int> (intersectionName);

    Intersection i = intersections[indx];

    if (i.name == 0)
        return -1; //intersection does not exist

    if(i.isGreen) //if the signal is green that means red time left is zero
        return 0.00f;

    int timeInSecs = i.timeCounter; 
    float timeInMins = 0.00f;

    timeInMins = (-1*timeInSecs)%100;

    return timeInMins;

}