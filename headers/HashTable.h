#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>

struct IntersectionPair {

    char intersection1;
    char intersection2;

    IntersectionPair (char inter1, char inter2) ;

    bool operator==(const IntersectionPair& other) const ;

} ;


class HashTable {

    struct HashNode {

        IntersectionPair key;
        int carsOnRoad;
        HashNode *next;

        HashNode (const IntersectionPair& k, int v);

    } ;

    HashNode **table ; // Dynamic array of pointers to linked lists
    int arraySize ;

    int hash ( const &IntersectionPair pair) const;

    public:

    HashTable (int size = 100) ;

    ~HashTable () ;

    void insert ( char intersection1, char intersection2 , int carsOnRoad ) ;

    void remove ( char intersection1, char intersection2 ) ;

    int getNumOfCars ( char intersection1 ,char intersection2 ) const ;

};

#endif