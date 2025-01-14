#include "../headers/Graph.h"
#include "../headers/dijkstraMinHeap.h"
#include "../headers/TrafficSignal.h"

Graph::Graph():list(nullptr){}

void Graph::clearGraph(){
    GraphNode* current = list;
    while(current){
        //gets adjacent list to current node
        GraphNode* adjacent = current->adjacent;
        //empties the adjacent list
        while(adjacent){
            GraphNode* temp = adjacent->next;
            delete adjacent;
            adjacent = temp;
        }
        //deletes current node
        GraphNode* temp = current->next;
        delete current;
        current = temp;
    }
    list = nullptr;
}

Graph::~Graph(){
    clearGraph();
}

GraphNode* Graph::searchVertex(char name){
    GraphNode* current = list;
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

GraphNode* Graph::searchAdjacent(GraphNode* adjList, char name){
    GraphNode* current = adjList;
    while(current){
        if(current->name == name){
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void Graph::setAdjacent(int weight, GraphNode* vertex,char adjacent){
    if(!vertex){
        return;
    }
    GraphNode* adjList = vertex->adjacent;
    if(adjList == nullptr){
        vertex->adjacent = new GraphNode(weight,adjacent);
        return;
    }
    //iterate through adjacent list and insert at end
    while(adjList->next){
        adjList = adjList->next;
    }
    adjList->next = new GraphNode(weight,adjacent);
    
}

void Graph::display() const{
    GraphNode* vertex = list;
    int r = 3;
    mvprintw(0,0,"===== City Traffic Network =====");
    mvprintw(1,0,"Vertex -> Adjacent vertices");
    while(vertex){
        int c = 0;
        mvprintw(r,c,"%c -> ",vertex->name);
        c += 5;
        GraphNode* adjList = vertex->adjacent;
        while(adjList){
            mvprintw(r,c,"(%c, %d)",adjList->name,adjList->weight);
            c += 10;
            adjList = adjList->next;
        }
        vertex = vertex->next;
        r += 1;
    }

    refresh();
}

void Graph::displayBlockedRoads() const{
    GraphNode* vertex = list;
    int r = 2;
    mvprintw(0,0,"===== Blocked Roads =====");
    while(vertex){
        GraphNode* adjList = vertex->adjacent;
        while(adjList){
            if(adjList->weight == 1000){
                mvprintw(r,0,"%c -> %c",vertex->name,adjList->name);
                r+= 1;
            }
            adjList = adjList->next;
        }
        vertex = vertex->next;
    }

    refresh();
}

void Graph::setVertex(char name){
    GraphNode* current = list;
    //if first vertex, simply insert
    if(current == nullptr){
        list = new GraphNode(0,name);
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
    current->next = new GraphNode(0,name);
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
        GraphNode* vertex = searchVertex(vertexName);
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

void Graph::setRoadClosures(std::string file){
    std::fstream fileHandler(file,std::ios::in);
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
        GraphNode* vertex = searchVertex(vertexName);
        //get adjacent name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char adjacentName = data[0];
        //closure type
        getline(fileHandler,data,'\n');
        if(data.empty()) continue;
        //if there is no closure then iterate to next value and do not change anything
        if(data == "Clear"){
            continue;
        }
        if(vertex){
            //if vertex exists in graph, search of its desired adjacent vertex
            GraphNode* adjacent = searchAdjacent(vertex->adjacent,adjacentName);
            //if adjacent vertex is found
            if(adjacent){
                //if the road is under repair, put the weight of road to 500
                if(data == "Under Repair"){
                    adjacent->weight = 500;
                }
                //if the road is blocked, put the weight of road to 1000
                else if(data == "Blocked"){
                    adjacent->weight = 1000;
                }
            }
        }
    }
    fileHandler.close();
}

void Graph::vehicleRouting(char start, char end, int maxVert, TrafficSignal &signals){
    MinHeap heap(maxVert);
    int distances[maxVert];
    char previous[maxVert];
    bool visited[maxVert];

    for (int i = 0; i < maxVert; ++i) {
        distances[i] = heap.getMaxInt(); //initialize with max int value
        previous[i] = '\0';     //intialize prev array with null characters
        visited[i] = false;     //mark all vertices as unvisited
    }

    //initialize start distance with 0
    distances[start-'A'] = 0;
    //push it in min heap
    heap.insert(0,start);
    while(!heap.isEmpty()){
        //extract min from the min heap
        MinHeap::HeapQueueNode current = heap.extractMin();
        //check if current vertex is visited or not
        if(visited[current.vertex-'A']){
            continue;
        }
        //if its not visited, put it in visited
        visited[current.vertex-'A'] = true;
        //if current is the end, we break the loop
        if(current.vertex == end){
            break;
        }
        //search if vertex exists in graph
        GraphNode* vertex = searchVertex(current.vertex);
        if(!vertex){
            continue;
        }

        GraphNode* adj = vertex->adjacent;
        //goes through adjacency list
        while(adj){
            if(visited[adj->name-'A']){
                adj = adj->next;
                continue;
            }

            //calculates new distance based on weights
            int newDist = distances[current.vertex-'A'] + adj->weight + signals.getRedTimeLeft(current.vertex);
            //if the new calculated distance is smaller than original distance we update the distance and update the parent
            if(newDist < distances[adj->name-'A']){
                distances[adj->name-'A'] = newDist;
                previous[adj->name-'A'] = current.vertex;
                //insert it into the min heap
                heap.insert(newDist,adj->name);
            }

            adj = adj->next;
        }
    }

    char path[maxVert];
    int index = 0;
    //get the path from the previous array
    for(char v = end; v != '\0'; v = previous[v-'A']){
        path[index++] = v;
        if(v == start){
            break;
        }
    }

    //if path is invalid
    if (path[index - 1] != start) {
        erase();
        mvprintw(0, 0, "No path found from %c to %c", start, end);
        refresh();
        getch();
        return;
    }


    erase();
    mvprintw(0, 0, "Shortest path from %c to %c:", start, end);

    for (int i = index - 1; i >= 0; --i) {
        printw(" %c", path[i]);
        if (i > 0) printw(" ->");
    }

    if (distances[end - 'A'] >= 100) {
        //display as float
        printw("\nTotal distance: %.2f minutes", distances[end - 'A'] / 100.0f);
    } 
    else {
        //display as int
        printw("\nTotal distance: %d minutes", distances[end - 'A']);
    }
    refresh();
    getch();
}

void Graph::blockRoad(char st, char en){
    GraphNode* intersection = searchAdjacent(searchVertex(st)->adjacent,en);
    if(intersection){
        intersection->weight = 1000;
    }
    else{
        erase();
        printw("No such intersection found!");
        refresh();
        napms(500);
    }
}

std::string Graph::getAdjacencyList(char adj){

    GraphNode* current = searchVertex(adj);
    std::string list = "";
    while(current){
        list += current->name;
        current = current->next;
    }
    return list;
}

void Graph::vehicleRoutingUsingFile(std::string file, TrafficSignal& signals){
    std::fstream fileHandler(file,std::ios::in);
    if(!fileHandler){
        erase();
        mvprintw(0,0,"File not found!");
        refresh();
        return;
    }
    std::string data;
    getline(fileHandler,data); //to skip the headers
    while(!fileHandler.eof()){
        //get vehicle name
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char vehicleName = data[0];
        //get start
        getline(fileHandler,data,',');
        if(data.empty()) continue;
        char start = data[0];
        //get end
        getline(fileHandler,data,'\n');
        if(data.empty()) continue;
        //convert weight
        char end = data[0];
        erase();
        vehicleRouting(start,end,27,signals);
        refresh();
    }

    fileHandler.close();

}
