#ifndef GRAPH_H
#define GRAPH_H

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
    void setAdjacent(int weight,char vertex,char adjacent);
    void setVertex(char name);
    void clearGraph();
    ~Graph();

};

#endif