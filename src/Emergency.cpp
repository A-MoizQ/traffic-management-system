#include "Emergency.h"
    EmergencyQueue:: EmergencyQueue():EmergencyQueueList(nullptr){}

    
    int EmergencyQueue:: calculatePriorityLevel(string Priority){
        if (Priority == "High")
            return 30;
        else if (Priority == "Medium")
            return 15;
        else
            return 0;
    }

    void EmergencyQueue:: insertInEmergencyQueue(string vId, char startP, char vendPoint, string pLevel){
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
        cout << newNode->priorityLevel << " "; 
    }


    //  to delete the whole queue
    void EmergencyQueue:: deletEmergencyQueue(){
        EmergencyNode* backup;
        while (EmergencyQueueList != nullptr)
        {
            backup = EmergencyQueueList;
            EmergencyQueueList = EmergencyQueueList->next;
            delete backup;

        }
        
    }
    EmergencyQueue:: ~EmergencyQueue(){
        deletEmergencyQueue();
    }
