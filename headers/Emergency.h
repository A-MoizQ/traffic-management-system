#ifndef EMERGENCY_H
#define EMERGENCY_H
#include<string>
#include<ncurses.h>
#include<fstream>
#include"../headers/Graph.h"
using namespace std;
class EmergencyQueue
{
    struct EmergencyNode
    {
        string vehicleId;
        char startPoint;
        char endPoint;
        // 30 is for high, 15 is for medium ,and 0 is for medium
        int priorityLevel;
        EmergencyNode* next;
        EmergencyNode(){}
        EmergencyNode(int pLevel): priorityLevel(pLevel){}
        EmergencyNode(string vId, char startP, char vendPoint, int pLevel):next(nullptr){
            vehicleId = vId;
            startPoint = startP;
            endPoint = vendPoint;
            priorityLevel = pLevel;
        }
    };
    int calculatePriorityLevel(string Priority)const;
    EmergencyNode *EmergencyQueueList;
public:
    EmergencyQueue();
    void insertInEmergencyQueue(string vId, char startP, char vendPoint, string pLevel);
    void deletEmergencyQueue();
    ~EmergencyQueue();
    void removeFromEmergencyQueue();
    void loadFromEmergencyCsv(string fileName);
    EmergencyNode* getTop()const;
    bool isEmergencyQueueEmpty()const;
    void executeEmergencyVehicles(Graph* g);
    void aStarAlgorithm(char start, char end, string vehicleId, Graph* g);
    int getMaxInt()const; 
    int heuristicFunction(char start, char end,int edgeWeight)const;
    void insertAtEndInEmergencyQueue(string vId, char startP, char vendPoint, int plevel);
    void displayAStarPath(char* parent, char start, char end, string vehicleId, int cost);
};


#endif
