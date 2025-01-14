#ifndef CONGESTION_HASH_TABLE_H
#define CONGESTION_HASH_TABLE_H

#include <iostream>
#include<ncurses.h>
#include<fstream>

struct Road {

    char intersection1;
    char intersection2;

    Road () ; //to create an empty intersection road indicated by NUL

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
    int numOfRoads; //the number of roads set in the hashTable

    public:

    CongestionHashTable ( int congestionThreshold = 20, int size = 101) ; // using a prime number to get less collisions

    ~CongestionHashTable () ;

    void readFile(std::string fileName, WINDOW *win);

    int hash ( const Road& road) const;

    void insert ( char intersection1, char intersection2 , int carsOnRoad ) ;

    bool remove ( char intersection1, char intersection2 ) ;

    int getNumOfCars ( char intersection1 ,char intersection2 ) const ;

    int getSize() const;

    int getConThreshold() const;

    void displayRoadCongestion (WINDOW *win) const ;

    void updateRoad ( char intersection1, char intersection2, int carsOnRoad ) ;

    Road getRoadAtIndx( int indx ) const;

    Road getRoadLinearProbing( int indx ) const; //get the value using linear probing

    int getNumOfRoads() const;

};

#endif