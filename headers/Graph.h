#ifndef GRAPH_H
#define GRAPH_H
#include<string>
#include<fstream>
#include<ncurses.h>

class TrafficSignal;

struct GraphNode{
    int weight;
    char name;
    GraphNode* adjacent;
    GraphNode* next;
    GraphNode(int w,char n):weight(w),name(n),adjacent(nullptr),next(nullptr){}
};

class Graph{
    GraphNode* list;
public:
    Graph();
    void loadFromFile(std::string fileName);
    void display() const;
    void displayBlockedRoads() const;
    void setAdjacent(int weight,GraphNode* vertex,char adjacent);
    void setRoadClosures(std::string file);
    void setVertex(char name);
    GraphNode* searchVertex(char name);
    GraphNode* searchAdjacent(GraphNode* adjList,char name);
    void blockRoad(char st, char en);
    std::string getAdjacencyList(char adj);
    void clearGraph();
    void vehicleRouting(char start, char end,int maxVert, TrafficSignal &signals);
    void vehicleRoutingUsingFile(std::string file, TrafficSignal &signals);
    ~Graph();

};

#endif