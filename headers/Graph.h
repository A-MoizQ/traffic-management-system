#ifndef GRAPH_H
#define GRAPH_H
#include<string>
#include<fstream>
#include<ncurses.h>

class Graph{
    struct Node{
        int weight;
        char name;
        Node* adjacent;
        Node* next;
        Node(int w,char n):weight(w),name(n),adjacent(nullptr),next(nullptr){}
    };
    Node* list;
public:
    Graph();
    void loadFromFile(std::string fileName);
    void display() const;
    void setAdjacent(int weight,Node* vertex,char adjacent);
    void setVertex(char name);
    Node* searchVertex(char name);
    void clearGraph();
    ~Graph();

};

#endif