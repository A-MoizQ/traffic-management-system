#include "../headers/HashTable.h"


IntersectionPair::IntersectionPair ( char inter1, char inter2 ) :

        intersection1(inter1),
        intersection2(inter2)
         
        {}


bool IntersectionPair::operator== ( const IntersectionPair& other ) const {

    return intersection1 == other.intersection1 && intersection2 == other.intersection2;

}


HashTable::HashNode::HashNode ( const IntersectionPair& k, int v ):

    key(k),
    carsOnRoad(v),
    next(nullptr)
    
    {
        if( carsOnRoad < 0 )
            carsOnRoad = 0;
    }

void HashTable::HashNode::print () const {

    cout<<"\nIntersection 1: "<<key.intersection1;
    cout<<"\nIntersection 2: "<<key.intersection2;
    cout<<"\nCars on this road: "<<carsOnRoad;

};

HashTable::HashTable(int size) :

    arraySize(size)
    
    {

    if(arraySize <= 0)
        arraySize = 101;

    table = new HashNode*[ arraySize ];

    for ( int i = 0; i < arraySize; i++ ) {

        table[i] = nullptr; //initialize all indexes to nullptr

    }

}

HashTable::~HashTable() {

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

//hash function
int HashTable::hash(const IntersectionPair& pair) const {

    return ( pair.intersection1 + pair.intersection2 ) % arraySize ; //sum of ASCII values of the name of intersections kept between 0 and arraySize - 1

}


void HashTable::insert ( char intersection1, char intersection2, int carsOnRoad ) {

    IntersectionPair key(intersection1, intersection2);
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


bool HashTable::remove ( char intersection1, char intersection2 ) {

    IntersectionPair key(intersection1, intersection2);
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

    return false;

}

// Get the number of cars between two intersections
int HashTable::getNumOfCars (char intersection1, char intersection2) const {

    IntersectionPair key(intersection1, intersection2);
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

void HashTable::displayRoadCongestion () const {

    bool isEmpty = true;

    HashNode *current = nullptr;

    for(int i = 0; i<arraySize;  i++){

        current = table[i];

        while(current){

            current->print();

            current = current->next;

            isEmpty = false;

        }

    }

    if ( isEmpty )
        cout<<"\nNo road congestion data found\n";

}

IntersectionPair* HashTable::getCongestedRoads ( int congestionThreshold ) const {

    //count the total number of congested roads to determine the array size
    int count = 0;

    for (int i = 0; i < arraySize; i++) {

        HashNode* current = table[i];

        while (current) {

            if (current->carsOnRoad >= congestionThreshold) {
                count++;
            }

            current = current->next;

        }
    }

    //if there are no congested roads then return nullptr
    if(count==0)
        return nullptr;

    //allocate an array to hold the IntersectionPairs for congested roads
    IntersectionPair* congestedRoads = new IntersectionPair[count];

    //add IntersectionPairs that exceed the threshold
    int index = 0;
    for (int i = 0; i < arraySize; i++) {

        HashNode* current = table[i];

        while (current) {

            if (current->carsOnRoad >= congestionThreshold) {
                congestedRoads[index++] = current->key;
            }

            current = current->next;

        }
    }

    return congestedRoads;

}


void HashTable::updateRoad ( char intersection1, char intersection2, int carsOnRoad ) {

    if ( carsOnRoad < 0 )
        carsOnRoad = 0;

    IntersectionPair key(intersection1, intersection2);
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