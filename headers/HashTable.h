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

    //using chaining to overcome collisons of indexes
    HashNode **table ; // Dynamic array of pointers to linked lists i.e each index will be a linked list
    int arraySize ;

    int hash ( const &IntersectionPair pair) const;

    public:

    HashTable (int size = 101) ; // using a prime number to get less collisions

    ~HashTable () ;

    void insert ( char intersection1, char intersection2 , int carsOnRoad ) ;

    void remove ( char intersection1, char intersection2 ) ;

    int getNumOfCars ( char intersection1 ,char intersection2 ) const ;

    void displayRoadCongestion () const ;

    IntersectionPair* getCongestedRoads ( int congestionThreshold ) const ; //get all the roads where the number of vehicles is greater than or equal to the congestionThreshold

    void updateRoad ( char intersection1, char intersection2, int carsOnRoad ) ;

};

#endif