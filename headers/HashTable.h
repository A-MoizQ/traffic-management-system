#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>

using namespace std;

struct IntersectionPair {

    char intersection1;
    char intersection2;

    IntersectionPair(char inter1, char inter2) ;

}

class HashTable {

    struct HashNode {



    }
    int arraySize;

    int hash (const &IntersectionPair pair, int carsOnRoad) const;

    public:

    HashTable () ;

    ~HashTable () ;

    void insert ( char intersection1, char intersection2 , int carsOnRoad ) ;

    void remove ( char intersection1, char intersection2 ) ;

    int getNumOfCars ( char intersection1 ,char intersection2 ) const ;

};

#endif