#include "../headers/CongestionHashTable.h"


Road::Road () :

    //make an empty intersection pair
    intersection1(0),
    intersection2(0)

    {}

Road::Road ( char inter1, char inter2 ) :

        intersection1(inter1),
        intersection2(inter2)
         
        {}


bool Road::operator== ( const Road& other ) const {

    return intersection1 == other.intersection1 && intersection2 == other.intersection2;

}

void Road::operator= (const Road& other) {

    intersection1 = other.intersection1;
    intersection2 = other.intersection2;

}


CongestionHashTable::HashNode::HashNode ( const Road& k, int v ):

    key(k),
    carsOnRoad(v),
    next(nullptr)
    
    {
        if( carsOnRoad < 0 )
            carsOnRoad = 0;
    }

void CongestionHashTable::HashNode::print (WINDOW *win, int& line ) const {

    mvwprintw( win,line++, 1, "Intersection 1: %c", key.intersection1 );
    mvwprintw( win,line++, 1, "Intersection 2: %c", key.intersection2 );
    mvwprintw( win,line++, 1, "Cars on this road: %d", carsOnRoad );
    line++; // Add a blank line for better readability

}

CongestionHashTable::CongestionHashTable(std::string fileName, int congestionThreshold, int size) :

    arraySize(size)
    
    {

    if(arraySize <= 0)
        arraySize = 101;

    if(congestionThreshold <=0)
        congestionThreshold =1;

    table = new HashNode*[ arraySize ];

    for ( int i = 0; i < arraySize; i++ ) {

        table[i] = nullptr; //initialize all indexes to nullptr

    }

}

CongestionHashTable::~CongestionHashTable() {

    for (int i = 0; i < arraySize; i++) {

        HashNode* current = table[i];

        while (current) {

            HashNode* temp = current;
            current = current -> next;
            delete temp;

        }

    }

    delete[] table;
    table = nullptr;

}

void CongestionHashTable::readFile(std::string fileName, WINDOW *win) {

    std::fstream fileHandler(fileName, std::ios::in);
    if(!fileHandler){
        erase();
        mvprintw(win, 0,0, (fileName + " not found!").c_str() );
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

        //get intersection2 name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char intersection2Name = data[0];

        //generate random number of vehicles on road
        srand(time(0));
        int numOfCars = rand() % (congestionThreshold + rand() % congestionThreshold);

        //add in the congestion table
        insert(intersection1Name, intersection2Name, numOfCars);
    }
    fileHandler.close();

}


//hash function
int CongestionHashTable::hash(std::string fileName, const Road& pair) const {

    return ( pair.intersection1 + pair.intersection2 ) % arraySize ; //sum of ASCII values of the name of intersections kept between 0 and arraySize - 1

}


void CongestionHashTable::insert ( char intersection1, char intersection2, int carsOnRoad ) {

    if (carsOnRoad < 0) {
        carsOnRoad = 0; // Prevent negative cars count
    }

    Road key(intersection1, intersection2);
    int index = hash(key);

    HashNode* current = table[index];

    while (current) { //loop till we find an empty node 


        //if key already exists in the linked list then do nothing
        if (current -> key == key)
            return;

        current = current -> next;

    }

    //if key does not exist then, insert new node at the head of the list
    HashNode* newNode = new HashNode(key, carsOnRoad);
    newNode->next = table[index];
    table[index] = newNode;
    
}


bool CongestionHashTable::remove ( char intersection1, char intersection2 ) {

    Road key(intersection1, intersection2);
    int index = hash(key);

    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current) {

        if (current -> key == key) {

            if (prev) {
                prev->next = current->next;
            } 
            else {
                table[index] = current->next; //update head if first node is removed
            }

            delete current;
            return true;

        }

        prev = current;
        current = current->next;

    }

    //not found
    return false;

}

// Get the number of cars between two intersections
int CongestionHashTable::getNumOfCars (char intersection1, char intersection2) const {

    Road key(intersection1, intersection2);
    int index = hash(key);

    HashNode* current = table[index];

    while (current) {

        if (current -> key == key) {
            return current -> carsOnRoad; // Key found
        }

        current = current -> next;

    }

    return -1; //if intersection pair was not found
}

int CongestionHashTable::getSize () const {

    return arraySize;

}

int CongestionHashTable::getConThreshold() const {

    return congestionThreshold;

}

void CongestionHashTable::displayRoadCongestion(WINDOW *win) const {

    bool isEmpty = true;
    HashNode* current = nullptr;
    int line = 1; //start printing at line 1 in the ncurses window

    mvwprintw(win, line++, 1, "=== Road Congestion Report ===");

    for (int i = 0; i < arraySize; i++) {

        current = table[i];

        while (current) {

            //print road data
            current->print(win,line);

            //highlight congested roads
            if (current->carsOnRoad >= congestionThreshold) {
                mvwprintw(win,line++, 1, "  --> CONGESTED (Cars: %d)", current->carsOnRoad);
            }

            current = current->next;
            isEmpty = false;

        }

    }

    if (isEmpty) {
        mvwprintw(win, line++, 1, "No road congestion data found.");
    }

    wrefresh(win); // Refresh ncurses window

}


void CongestionHashTable::updateRoad ( char intersection1, char intersection2, int carsOnRoad ) {

    if ( carsOnRoad < 0 )
        carsOnRoad = 0;

    Road key(intersection1, intersection2);
    int index = hash(key);

    HashNode* current = table[index];

    while (current) { //loop till the end of list

        //if key found then update
        if (current -> key == key) {

            current -> carsOnRoad = carsOnRoad ;
            return;

        }

        current = current->next;

    }

    //if key does not exist then, insert new node at the head of the list
    HashNode* newNode = new HashNode(key, carsOnRoad);
    newNode->next = table[index];
    table[index] = newNode;
    
}

Road CongestionHashTable::getIntersectionAtIndx( int indx ) const{

    //indx out of range
    if ( indx >= arraySize || indx < 0 ){

        Road emptyRoad;
        return emptyRoad;

    }

    //no intersections exist at the index
    if ( table[indx] == nullptr ) {

        Road emptyRoad;
        return emptyRoad;

    }

    //get the node and return its key i.e Road
    return (table[indx])->key;

}


Road CongestionHashTable::getInterLinearProbing( int indx ) const{

    Road pair = getIntersectionAtIndx(indx);


    //if the index was not nul then return the obtained pair
    if(pair.intersection1 != 0 && pair.intersection2!=0) {
        return pair;

    }

    //if the index was empty then find the next valid index using linear probing
    for( int i = indx + 1; ;i++ ){

        //if reached the end of the array then loop from the start
        if( i>=arraySize )
            i = 0;

        //if we reach back to the original indx then the array is empty so return the empty pair
        if( i==indx )
            return pair;

        Road p = getIntersectionAtIndx(i);

        if( p.intersection1 != 0 && p.intersection2!=0 ) 
            return p;

    }

}