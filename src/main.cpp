#include<ncurses.h>
#include "../headers/Graph.h"
using namespace std;

int main(){
    initscr();
    noecho();               
    curs_set(0);            
    nodelay(stdscr, TRUE);  
    keypad(stdscr, TRUE); 
    int choice = -1;
    //make graph object
    Graph g;
    //load content from file into graph
    g.loadFromFile("./data/road_network.csv");
    do{
        int r = 0; int c = 0;
        //clears screen
        erase();
        mvprintw(r,c,"===== Simulation Dashboard =====");
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

            }
            else if(choice == 3){

            }
            else if(choice == 4){

            }
            else if(choice == 5){

            }
            else if(choice == 6){

            }
            else if(choice == 7){

            }
        }

        refresh();
        napms(150);

    }while(choice != 0);

    endwin();
    return 0;
}