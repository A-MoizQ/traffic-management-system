#include "../headers/TrafficSignal"

TrafficSignal::Intersection::Intersection(char _name, int _totalTime) : 

    name(_name) ,
    totalTime(_totalTime),
    isGreen(false),
    timeCounter(0),
    timeIncrement(0)

    {}


TrafficSignal::TrafficSignal() {

    //this is a kind of hash map which will store the intersection info
    //at the index equal to the ascii of the name of the intersection
    intersections = new char[127];

    for(int i = 0;i<127;i++){
        intersections[i] = '0'; //this indicates that the index is empty
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

TrafficSignal::getRandomValue() {

    return min + (rand() % (max - min + 1));

}

void reduceCongestion(char name, int extraCars, bool turnGreenInstantly = false;){

    Intersection i = intersections[ static_cast<int>name ];

    i.timeIncrement = extraCars * 2;
    

    //add the incremented time only when the light is green.
    //if the light is not green i.e red then do this:
    //when the timCounter is <= timeIncrement then turn the light green
    //dont wait till it goes to 0
    //this way we will decrease the red time as well
    if(turnGreenInstantly){
        i.isGreen = true;
        i.timeCounter = totalTime + i.timeIncrement;
    }


}

void update() {

    char randomIntersectionName = static_cast<char>(getRandomValue('A', 'Z'));
    char randomIntersectionName2 = static_cast<char>(getRandomValue('A', 'Z'));

    //get the road
    //randomly add or remove a car to the road
    //might need to implement a get road function in the congestionHashMap
    //might also need to implement a get congested roads function

}

