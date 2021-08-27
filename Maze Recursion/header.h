/*  Name: Keaton Lee
 *  Date: June 12, 2021
 *  Title: Maze Recusion
 *
 *  Description: Contains definitions as well as function prototypes
*/

// checks if header file was defined to avoid double inclusions
#ifndef HEADER_H
#define HEADER_H

// imports header files
#include "Maze.h"
#include "Button.h"
#include <string>
#include <vector>

/// Definitions

#define IRIS        al_map_rgb(93, 63, 211)
#define LAVENDER    al_map_rgb(230, 190, 255)
#define BLACK       al_map_rgb(0, 0, 0)
#define GREY        al_map_rgb(180, 180, 180)
#define GREEN       al_map_rgb(35, 178, 157)

/// Function Prototypes

// prototypes for functions in allegroFunctions.cpp
bool initializeAllegro();
void closeAllegro();

// prototypes for functions in menuFunctions.cpp
void playMenu();
void updateOptionsMenu(bool refresh, vector<Button> &buttonVector, int clicked);
bool mazeMenu(float speed, short selectedMaze, short selectedAlgorithm, string mazeText, string algorithmText);

#endif // HEADER_H
