#ifndef CONGESTION_HASH_TABLE_H
#define CONGESTION_HASH_TABLE_H

#include <iostream>
#include<ncurses.h>

struct Road {

    char intersection1;
    char intersection2;

    Road () ; //to create an empty intersection pair indicated by NUL

    Road (char inter1, char inter2) ;

    bool operator==(const Road& other) const ;
    void operator=(const Road& other) ;

} ;


class CongestionHashTable {

    struct HashNode {

        Road key;
        int carsOnRoad;
        HashNode *next;

        HashNode (const Road& k, int v);

        void print(WINDOW *win, int &line) const;

    } ;

    //using chaining to overcome collisons of indexes
    HashNode **table ; // Dynamic array of pointers to linked lists i.e each index will be a linked list
    int arraySize ;
    int congestionThreshold ; // A road is considered congested if num of cars on it are greater than this

    public:

    CongestionHashTable ( int congestionThreshold = 20, int size = 101) ; // using a prime number to get less collisions

    ~CongestionHashTable () ;

    void readFile(std::string fileName, WINDOW *win);

    int hash ( const Road& pair) const;

    void insert ( char intersection1, char intersection2 , int carsOnRoad ) ;

    bool remove ( char intersection1, char intersection2 ) ;

    int getNumOfCars ( char intersection1 ,char intersection2 ) const ;

    int getSize() const;

    int getConThreshold() const;

    void displayRoadCongestion (WINDOW *win) const ;

    void updateRoad ( char intersection1, char intersection2, int carsOnRoad ) ;

    Road getIntersectionAtIndx( int indx ) const;

    Road getInterLinearProbing( int indx ) const; //get the value using linear probing

};

#endif