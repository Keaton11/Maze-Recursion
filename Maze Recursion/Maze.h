/*  Name: Keaton Lee
 *  Date: June 12, 2021
 *  Title: Maze Recusion
 *
 *  Description: Protoype for Maze class
*/

// checks if header file was defined to avoid double inclusions
#ifndef MAZE_H
#define MAZE_H

// imports header files
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <queue>

using namespace std;

// defines colours
#define IRIS    al_map_rgb(93, 63, 211)
#define BLUE    al_map_rgb(173, 216, 230)
#define PURPLE  al_map_rgb(145, 30, 180)
#define GREEN   al_map_rgb(60, 179, 113)
#define RED     al_map_rgb(224, 0, 123)
#define BLACK   al_map_rgb(0, 0, 0)
#define PINK    al_map_rgb(255, 153, 204)

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_FONT *basic_font;

class Maze {

    public:
        /// Definitions
        // struct used for values using coordinates
        struct Coordinates {
            short x = 0;
            short y = 0;
        };

        // enum for directions (used to trace paths)
        typedef enum {
            UP,
            RIGHT,
            DOWN,
            LEFT,
            NONE,
        } Direction;

        /// Constructors
        Maze(float s);
        Maze() {};

        /// Destructor
        ~Maze() {};

        /// Public Method Prototypes
        void fillMazeFile(const char fileName[]);
        void fillMazeGenerate(short n);
        void drawMaze();
        bool findPath(short algorithm);

    private:
        /// Private Variables
        short rows;     // number of rows in the maze
        short columns;  // number of columns in the maze

        float speed;    // corresponds to the animation speed (for maze generation and algorithms)

        vector<vector<char>> mazeChars;     // contains maze characters which correspond to the starting position, goal position, empty spaces, and obstacles

        vector<vector<bool>> marked;        // used to keep track of which spaces on the maze have been marked
        vector<vector<bool>> visited;       // used to keep track of which spaces on the maze have been visited
        vector<vector<bool>> holes;         // used to keep track of where holes have been drawn during maze generation

        vector<vector<bool>> coloured;      // used to colour the shortest path determined by the selected algorithm

        vector<vector<float>> pathCost;     // contains the calculated path cost to each space on the maze
        vector<vector<float>> gCost;        // contains the calculated g cost to each space on the maze

        vector<vector<vector<Direction>>> pathBefore;   // contains the direction(s) followed to get to each space on the maze

        queue<Coordinates> nodeQueue;       // queue used to keep track of which space the BFS algorithm should visit next

        Coordinates mazeCorner; // coordinates of the top left corner of the maze
        Coordinates mazeSize;   // dimensions of the maze in terms of pixel size

        Coordinates start;      // coordinates of the starting position in the maze
        Coordinates goal;       // coordinates of the goal position in the maze

        Coordinates current;    // coordinates of the space in the maze which the path is currently on

        /// Private Method Prototypes
        // prototypes for methods used to resize and reset all the vectors
        void resizeVectors();
        void resetVectors(int x, int y);

        // prorotypes for methods used for recursive maze generation
        void recursiveMazeGeneration(short width, short height, Coordinates offset);
        void updateMazeGeneration(bool h, short wall, short hole, short width, short height, Coordinates offset);

        // prototype for method used to find the starting position and the goal position in the maze
        void findStartAndGoal();

        // prototype for method used for DFS
        bool findPathDFS(short x, short y, Direction d);

        // prototypes for methods used for BFS
        bool findPathBFS();
        bool updateQueueBFS(short x, short y, Direction d);

        // prototypes of methods used for Dijkstra's algorithm
        bool findPathDijkstra();
        void updatePCDijkstra(int p, short x, short y, Direction d);

        // prototypes of methods used for A Star algorithm
        bool findPathAStar();
        void updatePCAStar(int g, short x, short y, Direction d);
        float hCostAStar(short x, short y);

        // prototypes of methods used for manual maze solcing (using arroy keys)
        bool findPathManual();
        bool updatePathManual(short x, short y, Direction d);

        // prototype for method used to determine the shortest path found by an algorithm
        void drawColouredPath(short x, short y);
};

#endif // MAZE_H
