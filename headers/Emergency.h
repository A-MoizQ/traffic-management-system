#ifndef EMERGENCY_H
#define EMERGENCY_H
#include<string>
#include <iostream>
#include<fstream>
class EmergencyQueue
{
    struct EmergencyNode
    {
        string vehicleId;
        char startPoint;
        char endPoint;
        // 30 is for high, 15 is for medium ,and 0 is for low
        int priorityLevel;
        EmergencyNode* next;
        EmergencyNode(){}
        EmergencyNode(int pLevel): priorityLevel(pLevel){}
        EmergencyNode(std::string vId, char startP, char vendPoint, int pLevel):next(nullptr){
            vehicleId = vId;
            startP = startPoint;
            endPoint = vendPoint;
            priorityLevel = pLevel;
        }
    };
    int calculatePriorityLevel(std::string Priority);
    EmergencyNode *EmergencyQueueList;
public:
    EmergencyQueue();
    void insertInEmergencyQueue(std::string vId, char startP, char vendPoint, std::string pLevel);
    void deletEmergencyQueue();
    void display();
    ~EmergencyQueue();
    void display();
    void removeFromEmergencyQueue();
    void loadFromEmergencyCsv(std:: string );
    


};


#endif
