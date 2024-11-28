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

Graph::Node* Graph::searchVertex(char name){
    Node* current = list;
    //iterate through vertex list
    while(current){
        //if vertex is found return it
        if(current->name == name){
            return current;
        }
        current = current->next;
    }
    //if vertex is not found return null
    return nullptr;
}

void Graph::setAdjacent(int weight, Node* vertex,char adjacent){
    if(!vertex){
        return;
    }
    Node* adjList = vertex->adjacent;
    if(adjList == nullptr){
        vertex->adjacent = new Node(weight,adjacent);
        return;
    }
    //iterate through adjacent list and insert at end
    while(adjList->next){
        adjList = adjList->next;
    }
    adjList->next = new Node(weight,adjacent);
    
}

void Graph::display() const{
    Node* vertex = list;
    int r = 3;
    mvprintw(0,0,"===== City Traffic Network =====");
    mvprintw(1,0,"Vertex -> Adjacent vertices");
    while(vertex){
        int c = 0;
        mvprintw(r,c,"%c -> ",vertex->name);
        c += 5;
        Node* adjList = vertex->adjacent;
        while(adjList){
            mvprintw(r,c,"(%c, %d)",adjList->name,adjList->weight);
            c += 7;
            adjList = adjList->next;
        }
        vertex = vertex->next;
        r += 1;
    }

    refresh();
}

void Graph::setVertex(char name){
    Node* current = list;
    //if first vertex, simply insert
    if(current == nullptr){
        list = new Node(0,name);
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


void Graph::loadFromFile(std::string fileName){
    std::fstream fileHandler(fileName, std::ios::in);
    if(!fileHandler){
        erase();
        mvprintw(0,0,"File not found!");
        refresh();
        return;
    }
    std::string data;
    getline(fileHandler,data); //to skip the headers
    while(!fileHandler.eof()){
        //get vertex name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char vertexName = data[0];
        //search if vertex exists
        Node* vertex = searchVertex(vertexName);
        //get adjacent name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char adjacentName = data[0];
        //get weight
        getline(fileHandler,data,'\n');
        if(data.empty()) continue;
        //convert weight
        int weight = stoi(data);
        //if vertex does not exist, set vertex then get it
        if(vertex == nullptr){
            setVertex(vertexName);
            vertex = searchVertex(vertexName);
        }

        setAdjacent(weight,vertex,adjacentName);

    }
    fileHandler.close();
}   