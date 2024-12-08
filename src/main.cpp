#include<ncurses.h>
#include "../headers/Graph.h"
#include "../headers/CongestionHashTable.h"
#include "../headers/TrafficSignal.h"
#include "../headers/Emergency.h"

using namespace std;


int main(){

    //ERRORS A RAHAY HAINNNNNNN😭😭😭😭😭
    WINDOW *win = initscr();
    noecho();               
    curs_set(0);            
    nodelay(stdscr, TRUE);  
    keypad(stdscr, TRUE); 
    int choice = -1;
    //make graph object
    Graph g;
    //load content from file into graph
    g.loadFromFile("./data/road_network.csv");
    g.setRoadClosures("./data/road_closures.csv");

    //assume the names of the intersection will be from A-Z which is total 27 names
    //for a road 2 of these inter. are needed so there are possible 27 * 27 = 729 cominations for the name of the road
    //we will use 727 as the size of the hashtable bcz its a prime number and will have minimum collisions
    //pass the congestion threshold and size of the hash map
    CongestionHashTable congestion(5, 727);
    congestion.readFile("./data/road_network.csv", win); //pass the window pointer to display error message
    /*
    Issues:
    -In vehicles.csv only the start and end intersections are given, we dont know which road the car will start on
    so will have to figure out a way to do that
    -one of the ways could be to pass the hashtable reference in the graph, and as the shortest path for each car is being calculated, we will get the first 2 intersections in that path and increment the numOfCars on the road by 1 */

    TrafficSignal signals(congestion);
    signals.readTrafficSignalFile("./data/traffic_signals.csv", win); //pass the window pointer to display error message

    do{
        int r = 0; int c = 0;
        //clears screen
        erase();
        mvprintw(r,c,"===== Simulation Dashboard =====");

        signals.displayTraffic(win , r); //display the traffic signals status and road congestion
        signals.updateCongestion(15); //update the congestion levels of 15 random roads
        signals.updateTime(); //update the time of all signals
        
        r++;
        mvprintw(r,c,"[0]. Exit");
        r++;
        mvprintw(r,c,"[1]. Display City Traffic Network");
        r++;
        mvprintw(r,c,"[2]. Display Traffic Signal Status");
        r++;
        mvprintw(r,c,"[3]. Display Congestion Status");
        r++;
        mvprintw(r,c,"[4]. Display Blocked Roads");
        r++;
        mvprintw(r,c,"[5]. Handle Emergency Vehicle Routing");
        r++;
        mvprintw(r,c,"[6]. Block Road Due to Accident");
        r++;
        mvprintw(r,c,"[7]. Simulate Vehicle Routing");
        r++;
        choice = getch() - '0';
        // choice = 5;
        if(choice != 0){
            if(choice == 1){
                //clear screen
                erase();
                //display the graph
                g.display();
                //disable non blocking input
                nodelay(stdscr, FALSE); 
                //wait for user to press a key
                getch();
                //re enable non blocking input
                nodelay(stdscr, TRUE); 
                choice = -1; //reset choice to allow for non blocking input
            }
            else if(choice == 2){

                //clear screen
                erase();
                //display the graph
                signals.displaySignals(win, r);
                //disable non blocking input
                nodelay(stdscr, FALSE); 
                //wait for user to press a key
                getch();
                //re enable non blocking input
                nodelay(stdscr, TRUE); 
                choice = -1; //reset choice to allow for non blocking input

            }
            else if(choice == 3){
                //clear screen
                erase();
                //display the congestion
                congestion.displayRoadCongestion(win);
                //disable non blocking input
                nodelay(stdscr, FALSE); 
                //wait for user to press a key
                getch();
                //re enable non blocking input
                nodelay(stdscr, TRUE); 
                choice = -1; //reset choice to allow for non blocking input
            }
            else if(choice == 4){
                //clear screen
                erase();
                //display blocked roads
                g.displayBlockedRoads();
                //disable non blocking input
                nodelay(stdscr, FALSE); 
                //wait for user to press a key
                getch();
                //re enable non blocking input
                nodelay(stdscr, TRUE); 
                choice = -1; //reset choice to allow for non blocking input
            }
             else if(choice == 5){
                //clear screen
                erase();
                //disable non blocking input
                nodelay(stdscr, FALSE); 
                EmergencyQueue em;
                em.loadFromEmergencyCsv("./data/emergency_vehicles.csv");
                em.executeEmergencyVehicles(&g);
                //wait for user to press a key
                getch();
                //re enable non blocking input
                nodelay(stdscr, TRUE); 
                choice = -1; //reset choice to allow for non blocking input
            }
            else if(choice == 6){

            }
            else if(choice == 7){
                erase();
                nodelay(stdscr,FALSE);
                mvprintw(0,0,"Enter start point: ");
                echo();
                refresh();
                char start = getch();
                mvprintw(1,0,"Enter End Point: ");
                refresh();
                char end = getch();
                noecho();
                g.vehicleRouting(start,end,27, signals);
                nodelay(stdscr,TRUE);
                choice = -1;
            }
        }
        refresh();
        napms(150);

    }while(choice != 0);

    endwin();
    return 0;
}