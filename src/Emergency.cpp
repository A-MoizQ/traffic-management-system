#include "../headers/Emergency.h"
#include<iostream> 
using namespace std;
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
            mvprintw(0,0,"File not found!");
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
                char startPoint = ' ', endPoint = ' ';
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

    EmergencyQueue:: EmergencyNode* EmergencyQueue:: getTop(){
        if (EmergencyQueueList == nullptr)
        {
            return nullptr;
        }
        else{
            return EmergencyQueueList;
        }
        
    }