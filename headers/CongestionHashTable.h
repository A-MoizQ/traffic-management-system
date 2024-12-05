#ifndef CONGESTION_HASH_TABLE_H
#define CONGESTION_HASH_TABLE_H

#include <iostream>

struct IntersectionPair {

    char intersection1;
    char intersection2;

    IntersectionPair (char inter1, char inter2) ;

    bool operator==(const IntersectionPair& other) const ;

} ;


class CongestionHashTable {

    struct HashNode {

        IntersectionPair key;
        int carsOnRoad;
        HashNode *next;

        HashNode (const IntersectionPair& k, int v);

        void print(int &line) const;

    } ;

    //using chaining to overcome collisons of indexes
    HashNode **table ; // Dynamic array of pointers to linked lists i.e each index will be a linked list
    int arraySize ;
    int congestionThreshold ; // A road is considered congested if num of cars on it are greater than this

    int hash ( const IntersectionPair& pair) const;

    public:

    CongestionHashTable (int congestionThreshold = 20, int size = 101) ; // using a prime number to get less collisions

    ~CongestionHashTable () ;

    void insert ( char intersection1, char intersection2 , int carsOnRoad ) ;

    bool remove ( char intersection1, char intersection2 ) ;

    int getNumOfCars ( char intersection1 ,char intersection2 ) const ;

    void displayRoadCongestion () const ;

    void updateRoad ( char intersection1, char intersection2, int carsOnRoad ) ;

};

#endif