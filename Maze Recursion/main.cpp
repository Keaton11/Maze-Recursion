/****************************************************************************
 *  Name: Keaton Lee                                         June 12, 2021  *
 *  Title: Maze Recusion                                                    *
 *                                                                          *
 *  Description: This program recieves input data of a maze from a text     *
 *  file. The maze contains a starting position and a goal position, which  *
 *  the program will navigate using different recursive path finding        *
 *  algorithms. Positions in the maze will either be open or blocked with   *
 *  an obstacle. The program can also use a recursive method to generate    *
 *  new mazes. The user can select the type of maze (the maze from the      *
 *  input file or a randomly generated maze), the speed (of the animation   *
 *  for the algorithm and the maze generation), and the algorithm (DFS,     *
 *  BFS, Dijkstra, A Star, or Manual (using the arrow keys).                *
 ****************************************************************************/

// imports header files
#include "header.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// global allegro variables
ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_FONT *basic_font = nullptr;
ALLEGRO_FONT *title_font = nullptr;
ALLEGRO_TIMER *timer = nullptr;

int main() {

    // seeds rand with a starting value to allow for a random generation of numbers
    srand(time(0));

    // calls function to initialize allegro
    if (!initializeAllegro()) {
        // if allegro fails to initialize, 1 is returned
        return 1;
    }

    // calls play function which loops until user exits the assignment
    // this allows the user to run multiple algorithms with various mazes
    playMenu();

    // calls function to close allegro
    closeAllegro();

    return 0;
}
