#ifndef EMERGENCY_H
#define EMERGENCY_H
#include<string>
#include<ncurses.h>
#include<fstream>
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
            startP = startPoint;
            endPoint = vendPoint;
            priorityLevel = pLevel;
        }
    };
    int calculatePriorityLevel(string Priority);
    EmergencyNode *EmergencyQueueList;
public:
    EmergencyQueue();
    void insertInEmergencyQueue(string vId, char startP, char vendPoint, string pLevel);
    void deletEmergencyQueue();
    ~EmergencyQueue();
    void removeFromEmergencyQueue();
    void loadFromEmergencyCsv(std:: string );
    


};


#endif
