#include "../headers/Graph.h"

Graph::Graph():list(nullptr){}

void Graph::clearGraph(){
    Node* current = list;
    while(current){
        //gets adjacent list to current node
        Node* adjacent = current->adjacent;
        //empties the adjacent list
        while(adjacent){
            Node* temp = adjacent->next;
            delete adjacent;
            adjacent = temp;
        }
        //deletes current node
        Node* temp = current->next;
        delete current;
        current = temp;
    }
    list = nullptr;
}

Graph::~Graph(){
    clearGraph();
}

void Graph::setAdjacent(int weight, char vertex,char adjacent){
    Node* current = list;
    while(current){
        //if vertex is found
        if(current->name == vertex){
            Node* adjList = current->adjacent;
            //iterate through adjacent list and insert at end
            while(adjList->next){
                adjList = adjList->next;
            }
            adjList->next = new Node(weight,adjacent);
        }
    }
}

void Graph::setVertex(char name){
    Node* current = list;
    //if first vertex, simply insert
    if(current == nullptr){
        current = new Node(0,name);
        return;
    }
    //else iterate through the vertex list
    while(current->next){
        //if vertex already exists, return
        if(current->name == name){
            return;
        }
        current = current->next;
    }
    //insert at end of vertex list
    current->next = new Node(0,name);
}