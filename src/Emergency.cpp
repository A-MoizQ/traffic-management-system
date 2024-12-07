#include "../headers/Emergency.h"
#include<iostream> 
using namespace std;
    EmergencyQueue:: EmergencyQueue():EmergencyQueueList(nullptr){}

    
    int EmergencyQueue:: calculatePriorityLevel(string Priority)const
    {
        if (Priority == "High")
            return 30;
        else if (Priority == "Medium")
            return 15;
        else
            return 0;
    }

    void EmergencyQueue:: insertInEmergencyQueue(string vId, char startP, char vendPoint, string pLevel)
    {
        EmergencyNode* newNode = new EmergencyNode( vId,startP,vendPoint,calculatePriorityLevel(pLevel));
        EmergencyNode* backup = EmergencyQueueList;

        /* 
            We have 2 cases to add at the head if the priority is greater or the list is empty
        */
        if (EmergencyQueueList == nullptr || newNode->priorityLevel > EmergencyQueueList->priorityLevel)  
        {

            newNode->next = EmergencyQueueList;
            EmergencyQueueList = newNode;
        }
        else{
            while (backup->next != nullptr && backup->next->priorityLevel >= newNode->priorityLevel)
            {
                backup = backup->next;
            }
            newNode->next = backup->next;
            backup->next = newNode;
        }  
    }


    //  to delete the whole queue
    void EmergencyQueue:: deletEmergencyQueue(){
        EmergencyNode* backup;
        while (EmergencyQueueList != nullptr){
            backup = EmergencyQueueList;
            EmergencyQueueList = EmergencyQueueList->next;
            delete backup;

        }
        
    }
    EmergencyQueue:: ~EmergencyQueue(){
        deletEmergencyQueue();
    }


    void EmergencyQueue:: removeFromEmergencyQueue(){
        if (EmergencyQueueList == nullptr){
            erase();
            mvprintw(0,0,"Queue is empty!");
            refresh();
            napms(1000);
            return;
        }
        else{
            EmergencyNode* backup = EmergencyQueueList;
            EmergencyQueueList = EmergencyQueueList->next;
            delete backup;
        }
    }

    void EmergencyQueue::loadFromEmergencyCsv(string fileName){
        fstream fileHandler(fileName, ios::in);
        if(!fileHandler){
            erase();
            mvprintw(0,0,"Emergency Csv not found!");
            refresh();
            return;
        }
        string data;
        getline(fileHandler,data); //to skip the headers
        while(!fileHandler.eof()){
            getline(fileHandler,data);
            if (data[0] == ',')
            {
                break;
            }
            else
            {    // to check if the vehicle id is present in the file
                int j = 0;
                string vId = "",priorityLevel = "";
                char startPoint = ' ';
                char endPoint = ' ';
                for (int i = 0; data[i] != '\0'; i++)
                {
                    if (data[i] == ',')
                    {
                        j++;
                    }
                    else{
                        if (j == 0)
                        {
                            vId += data[i];   
                        }
                        else if (j == 1)
                        {
                            startPoint = data[i];
                        }
                        else if (j == 2)
                        {
                            endPoint = data[i];
                        }
                        else{
                            priorityLevel += data[i];
                        }
                    }
                }
                insertInEmergencyQueue(vId,startPoint,endPoint,priorityLevel);
            }
        }
        fileHandler.close();
    }

    EmergencyQueue:: EmergencyNode* EmergencyQueue:: getTop()const{
        if (EmergencyQueueList == nullptr)
        {
            return nullptr;
        }
        else{
            return EmergencyQueueList;
        }
        
    }

    bool EmergencyQueue:: isEmergencyQueueEmpty()const{
        return EmergencyQueueList == nullptr;
    }

    int EmergencyQueue::getMaxInt() const
    {
        return (1 << (sizeof(int) * 8 - 1)) - 1;
    }

    void EmergencyQueue::insertAtEndInEmergencyQueue(string vId, char startP, char vendPoint, int plevel){
         EmergencyNode* newNode = new EmergencyNode( vId,startP,vendPoint,plevel);
        EmergencyNode* backup = EmergencyQueueList;
        if (EmergencyQueueList == nullptr)
        {
            EmergencyQueueList = newNode;
        }
        else{
            while (backup->next != nullptr)
            {
                backup = backup->next;
            }
            backup = newNode;
        }
    }

    void EmergencyQueue::displayAStarPath (char* parent, char start, char end, string vehicleId, int cost) 
    {
        char path[27];
        int pathIdx = 0;
        char current = end;

        while (current != '\0') 
        {
            path[pathIdx++] = current;
            current = parent[current - 'A'];//move to the parent of the current node
        }
    
        if (path[pathIdx - 1] != start) // the last node must be present in the array then path will be valid
        {
            printw( "No path found for vehicle %s from %c to %c \n\n ", vehicleId.c_str() , start,end);
            return;
        }

        printw("Vehicle %s  path: ",vehicleId.c_str());
        for (int i = pathIdx - 1; i >= 0; i--) 
        {
            printw(" %c  ", path[i]);
        }
        printw("\nTotal travel time: %d minutes\n", cost);
    }

    int EmergencyQueue::heuristicFunction(char start, char end,int weight)const
    {
        return abs(start - end) + weight;
    }


    void EmergencyQueue::aStarAlgorithm( char start, char end, string vehicleId, Graph* g) 
    {
        const int maxVertices = 27;
        struct aStarNode {
            bool closedList[27];//to mark those vertices who are vistied
            int gCost[27];//cost from start node to the end node
            int fCost[27];//esitmated total cost gcost + heuristic cost
            char parent[27];//Stores the parent of each node to reconstruct the shortest path.
            EmergencyQueue openList;//to store nodes to be explored, ordered by their fCost
        };
        aStarNode aStarNode1; 
        for (int i = 0; i < maxVertices; i++) 
        {
            aStarNode1.closedList[i] = false;
            aStarNode1.gCost[i] = this->getMaxInt();
            aStarNode1.fCost[i] = this->getMaxInt(); 
            aStarNode1.parent[i] = '\0';
        }
        int startIdx = start - 'A';//start index
        int endIdx = end - 'A';//end index
        aStarNode1.gCost[startIdx] = 0;// as the cost for itself is zero
        aStarNode1.fCost[startIdx] = this->heuristicFunction(start,end,0);
        aStarNode1.openList.insertAtEndInEmergencyQueue(vehicleId,start,end,aStarNode1.fCost[startIdx]);
        while (!aStarNode1.openList.isEmergencyQueueEmpty()) 
        {
            char current = (aStarNode1.openList.getTop()->startPoint);
            aStarNode1.openList.removeFromEmergencyQueue();
            int currentIdx = current - 'A';
            if (current == end)
            {
                this->displayAStarPath(aStarNode1.parent, start, end, vehicleId, aStarNode1.gCost[endIdx]);
                return;
            }
            aStarNode1.closedList[currentIdx] = true;// the node is visited

            GraphNode* currentNode = g->searchVertex(current);
            if (!currentNode)
            {
                printw("Error: Vertex %c is not found in the graph ",current);
                return;
            }
            GraphNode* edge = currentNode->adjacent;
            if (!currentNode)
            {
                printw("No adjacent vertex to  %c is  found in the graph ",current);
                return;
            }
            // set the direct weight from the start to the nodes that are neighbor to the current node
            while (edge) {
                int weight = edge->weight;
                if (weight == 500 || weight == 1000) // to skip those edges where the weight is 1000(blocked road) or 500(road is in repaird)
                {
                    edge = edge->next;
                    continue;
                }
                char neighbor = (edge->name);
                int neighborIdx = neighbor - 'A';

                if (aStarNode1.closedList[neighborIdx])
                {
                    edge = edge->next;
                    continue;
                }

                int tentativeGCost = aStarNode1.gCost[currentIdx] + weight;
                // tentative ocst to reach from start to the neighbor of the current node
                if (tentativeGCost < aStarNode1.gCost[neighborIdx]) 
                {
                    //as gcost is infinity at start that will be updated till it become minimum
                    aStarNode1.gCost[neighborIdx] = tentativeGCost;
                    aStarNode1.fCost[neighborIdx] = tentativeGCost +this->heuristicFunction(current,neighbor,weight);
                    aStarNode1.parent[neighborIdx] = current;
                    aStarNode1.openList.insertAtEndInEmergencyQueue(vehicleId,neighbor,currentNode->name,aStarNode1.fCost[neighborIdx]);// using this as a FIFO order
                }
                edge = edge->next;
            }
        }       
        printw("No path found for vehicle %s from %c to %c \n", vehicleId.c_str(),start, end );
    }

    void EmergencyQueue::executeEmergencyVehicles(Graph* g){

        while ((!this->isEmergencyQueueEmpty()))
        {
            aStarAlgorithm(this->getTop()->startPoint, this->getTop()->endPoint ,this->getTop()->vehicleId ,g);
            this->removeFromEmergencyQueue();
            refresh();
            getch();
            erase();
        }
    }