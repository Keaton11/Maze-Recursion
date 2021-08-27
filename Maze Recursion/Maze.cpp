/*  Name: Keaton Lee
 *  Date: June 12, 2021
 *  Title: Maze Recusion
 *
 *  Description: Holds the implementations of the methods associated with the
 *  Maze class
*/

// includes header file
#include "Maze.h"

/// Constructor
// recieves float corresponding to speed as parameters
// sets default values for the maze corner and maze size, then sets speed based on parameters
Maze::Maze(float s) {
    mazeCorner.x = 100;
    mazeCorner.y = 130;
    mazeSize.x = 500;
    mazeSize.y = 500;
    speed = s;
}

/// Method Prototypes - File Input
// recieves the name of a file as parameters
// reads data from the file, and sets maze accordingly
void Maze::fillMazeFile(const char fileName[]) {
    // opens file stream
    ifstream inputFile(fileName);
    if (!inputFile.is_open()){
        // prints error message is file is not present
        cout << "Error: Unable to open file" << endl;
    }

    // determines the size of the maze by reading data from the file
    rows = 0;
    string columnNum;
    while (!inputFile.eof()) {
        getline(inputFile, columnNum);
        if (!columnNum.empty()) {
            // variables for the number of rows and columns are updated
            rows++;
            columns = columnNum.length();
        }
    }
    // calls method to resize vectors based on the number of rows and columns
    resizeVectors();

    // clears bad state after eof
    inputFile.clear();
    // returns to the beginning of the file
    inputFile.seekg(0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            // reads maze and stores values in mazeChars vector
            inputFile >> mazeChars[j][i];
            // calls method to reset vectors
            resetVectors(j, i);
        }
    }

    //closes input file
    inputFile.close();
}

/// Method Prototypes - Resize / Reset Vectors
// recieves no parameters
// resizes vectors based on the values of rows and columns
void Maze::resizeVectors() {
    mazeChars.resize(columns, vector<char>(rows));
    marked.resize(columns, vector<bool>(rows));
    visited.resize(columns, vector<bool>(rows));
    holes.resize(columns, vector<bool>(rows));
    coloured.resize(columns, vector<bool>(rows));
    pathCost.resize(columns, vector<float>(rows));
    gCost.resize(columns, vector<float>(rows));
    pathBefore.resize(columns, vector<vector<Direction>>(rows));
    while (!nodeQueue.empty()) nodeQueue.pop();
}
// recieves an x and y coordinate as parameters
// sets vectors to default values at given x and y coordinate
void Maze::resetVectors(int x, int y) {
    marked[x][y] = false;
    visited[x][y] = false;
    holes[x][y] = false;
    coloured[x][y] = false;
    pathCost[x][y] = INT_MAX;
    gCost[x][y] = INT_MAX;
    pathBefore[x][y].clear();
}

/// Method Prototypes - Recursive Maze Generation
// recieves a short(n) as parameters
// selects a random starting position and goal position, then calls method to generate a maze of size n * n
void Maze::fillMazeGenerate(short n) {
    // the number of rows and columns is set based on the given parameters
    rows = n;
    columns = n;
    // calls method to resize vectors based on the number of rows and columns
    resizeVectors();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            // sets all coordinates of maze to non-obstacle spaces
            mazeChars[j][i] = '.';
            // calls method to reset vectors
            resetVectors(j, i);
        }
    }

    // randomizes y coordinates of the starting position and the goal position
    start.y = rand() % (rows - 1);
    goal.y = rand() % (rows - 1);
    // updates mazeChars vector by placing starting and goal position
    mazeChars[0][start.y] = 'S';
    mazeChars[columns - 1][goal.y] = 'G';
    // sets current position to starting position
    current.x = 0;
    current.y = start.y;

    // calls recursive method to generate maze
    Coordinates offset; // declares offset variable which is initialized to the default Coordinate values
    recursiveMazeGeneration(rows, columns, offset);
}
// recieves shorts corresponding to width and height as well as a variable corresponding to the offset value as parameters
// generates a maze through recursive division (by repeatedly splitting the maze with walls containing a hole)
void Maze::recursiveMazeGeneration(short width, short height, Coordinates offset) {
    if (width < 3 || height < 3) {
        // method returns if width or height are less than 3
        return;
    }

    // boolean value determining whether the split will be horizontal or vertical is randomized
    bool horizontal = rand() % 2;
    if (width < height) {
        // if width is less than height, the split will be horizontal
        horizontal = true;
    } else if (height < width) {
        // if height is less than width, the split will be vertical
        horizontal = false;
    }

    // variables are declared
    short wall, hole;   // shorts correspond to the placement of the hole and wall
    int counter = 0;    // counter is used to limit the number of while loop iterations

    if (horizontal) {
        while (true) {
            if (counter++ > 5) {
                // method returns if counter surpases 5 as it is likely that no wall can be placed due to the position of holes
                return;
            }
            // y value for wall is randomly selected
            wall = rand() % (height - 2) + 1;
            // loop restarts if the wall intersects with a hole
            if (offset.x != 0) {
                if (holes[offset.x - 1][wall + offset.y])
                    continue;
            }
            if (offset.x + width != columns) {
                if (holes[offset.x + width][wall + offset.y])
                    continue;
            }
            break;
        }
        // x value for hole is randomly selected
        hole = rand() % (width - 2) + 1;
        holes[hole + offset.x][wall + offset.y] = true;

        // calls method to update maze generation
        updateMazeGeneration(horizontal, wall, hole, width, height, offset);
        // method calls itself to add more walls above the new horizontal wall
        recursiveMazeGeneration(width, wall, offset);
        offset.y += wall + 1;   // offset is updated based on wall placement
        // method calls itself to add more walls below the new horizontal wall
        recursiveMazeGeneration(width, height - wall - 1, offset);
    } else {
        while (true) {
            if (counter++ > 5) {
                // method returns if counter surpases 5 as it is likely that no wall can be placed due to the position of holes
                return;
            }
            // x value for wall is randomly selected
            wall = rand() % (width - 2) + 1;
            // loop restarts if the wall intersects with a hole
            if (offset.y != 0) {
                if (holes[wall + offset.x][offset.y - 1])
                    continue;
            }
            if (offset.y + height != rows) {
                if (holes[wall + offset.x][offset.y + height])
                    continue;
            }
            break;
        }
        // y value for hole is randomly selected
        hole = rand() % (height - 2) + 1;
        holes[wall + offset.x][hole + offset.y] = true;

        // calls method to update maze generation
        updateMazeGeneration(horizontal, wall, hole, width, height, offset);
        // method calls itself to add more walls to the left of the new vertical wall
        recursiveMazeGeneration(wall, height, offset);
        offset.x += wall + 1;   // offset is updated based on wall placement
        // method calls itself to add more walls to the right of the new vertical wall
        recursiveMazeGeneration(width - wall - 1, height, offset);
    }
}
// recieves boolean corresponding to horizontal/vertical, wall and hole position, width and height, and offset as parameters
// updates maze by adding the walls (and holes) to the mazeChars vector
void Maze::updateMazeGeneration(bool h, short wall, short hole, short width, short height, Coordinates offset) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if ((h && i == wall + offset.y && j != hole + offset.x) || (!h && j == wall + offset.x && i != hole + offset.y)) {
                if (i >= offset.y && i < height + offset.y && j >= offset.x && j < width + offset.x) {
                    if (mazeChars[j][i] == '.') {
                        // obstacles, represented by #, are placed where appropriate
                        mazeChars[j][i] = '#';
                    }
                }
            }
        }
    }
    // maze is redrawn
    drawMaze();
    // the variable speed determines how quickly the maze will be drawn
    al_rest(speed / 3);
}

/// Method Prototypes - Draw Maze
// recieves no parameters
// draws maze as well as paths
void Maze::drawMaze() {
    // the width and height of each maze box is calculated based on maze size and the number of rows and columns
    float boxWidth = mazeSize.x / (float)columns;
    float boxHeight = mazeSize.y / (float)rows;

    // maze border is drawn
    ALLEGRO_COLOR colour = IRIS;
    al_draw_filled_rectangle(90, 120, 610, 640, colour);

    // maze boxes are drawn above the maze border
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (mazeChars[j][i] == '.') {
                colour = BLUE;      // empty boxes re drawn in blue
            } else if (mazeChars[j][i] == '#') {
                colour = PURPLE;    // obstable boxes are drawn in purple
            } else if (mazeChars[j][i] == 'S') {
                colour = GREEN;     // the starting box is drawn in green
            } else if (mazeChars[j][i] == 'G') {
                colour = RED;       // the goal box is drawn in red
            }
            al_draw_filled_rounded_rectangle(mazeCorner.x + boxWidth * j + 2.5, mazeCorner.y + boxHeight * i + 2.5, mazeCorner.x + boxWidth * (j + 1) - 2.5, mazeCorner.y + boxHeight * (i + 1) - 2.5, 5, 5, colour);
        }
    }

    // paths are drawn aboce the maze boxes
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (coloured[j][i] && k == 1) {
                    // the final path for BFS, Dijkstra, and A Star is drawn in pink
                    // this path is drawn after the black paths
                    colour = PINK;
                } else if (!coloured[j][i] && k == 0) {
                    // by default, paths are drawn in black
                    colour = BLACK;
                } else {
                    continue;
                }

                if (marked[j][i] && !(start.x == j && start.y == i) || (mazeChars[j][i] == 'G' && marked[j][i])) {
                    // lines are drawn based on the direction the path travelled to arrive at the given coordinate
                    if (count(pathBefore[j][i].begin(), pathBefore[j][i].end(), UP))
                        al_draw_line(mazeCorner.x + boxWidth * (j + 0.5), mazeCorner.y + boxHeight * (i + 0.5) - 5, mazeCorner.x + boxWidth * (j + 0.5), mazeCorner.y + boxHeight * (i + 1.5) + 5, colour, 10);
                    if (count(pathBefore[j][i].begin(), pathBefore[j][i].end(), DOWN))
                        al_draw_line(mazeCorner.x + boxWidth * (j + 0.5), mazeCorner.y + boxHeight * (i - 0.5) - 5, mazeCorner.x + boxWidth * (j + 0.5), mazeCorner.y + boxHeight * (i + 0.5) + 5, colour, 10);
                    if (count(pathBefore[j][i].begin(), pathBefore[j][i].end(), LEFT))
                        al_draw_line(mazeCorner.x + boxWidth * (j + 0.5) - 5, mazeCorner.y + boxHeight * (i + 0.5), mazeCorner.x + boxWidth * (j + 1.5) + 5, mazeCorner.y + boxHeight * (i + 0.5), colour, 10);
                    if (count(pathBefore[j][i].begin(), pathBefore[j][i].end(), RIGHT))
                        al_draw_line(mazeCorner.x + boxWidth * (j - 0.5) - 5, mazeCorner.y + boxHeight * (i + 0.5), mazeCorner.x + boxWidth * (j + 0.5) + 5, mazeCorner.y + boxHeight * (i + 0.5), colour, 10);
                } else if (mazeChars[j][i] == 'S') {
                    // a circle is drawn on the starting box
                    al_draw_filled_circle(mazeCorner.x + boxWidth * (j + 0.5), mazeCorner.y + boxHeight * (i + 0.5), boxWidth * 0.3, colour);
                }
                // a circle is drawn on the current box
                al_draw_filled_circle(mazeCorner.x + boxWidth * (current.x + 0.5), mazeCorner.y + boxHeight * (current.y + 0.5), boxWidth * 0.3, colour);
            }
        }
    }
    // the allegro display is updated
    al_flip_display();
}

/// Method Prototypes - Algorithm Selection
// recieves a short corresponding to an algorithm as input
// calls the appropriate algorithm method
bool Maze::findPath(short algorithm) {
    // calls method to locate the starting position and the goal position
    findStartAndGoal();

    // returns true if any algorithm is completed
    if (algorithm == 0) {
        // if algorithm is equal to 0, the main DFS method is called
        if (findPathDFS(start.x, start.y, NONE))
            return true;
    } else if (algorithm == 1) {
        // if algorithm is equal to 1, the main BFS method is called
        nodeQueue.push(start);
        if (findPathBFS())
            return true;
    } else if (algorithm == 2) {
        // if algorithm is equal to 2, the main Dijkstra method is called
        pathCost[start.x][start.y] = 0;
        if (findPathDijkstra())
            return true;
    } else if (algorithm == 3) {
        // if algorithm is equal to 3, the main A Star method is called
        gCost[start.x][start.y] = 0;
        pathCost[start.x][start.y] = 0;
        if (findPathAStar())
            return true;
    } else if (algorithm == 4) {
        // if algorithm is equal to 4, the main Manual method is called
        if (findPathManual())
            return true;
    }
    // returns false if parameters were invalid or algorithm was not completed
    return false;
}
// recieves no parameters
// located the x and y position of the starting position and the goal position
void Maze::findStartAndGoal() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (mazeChars[j][i] == 'S') {
                start.x = j;
                start.y = i;
            } else if (mazeChars[j][i] == 'G') {
                goal.x = j;
                goal.y = i;
            }
        }
    }
}

/// Method Prototypes - Algorithm: DFS
// recieves an x value, a y value, and a direction as parameters
// uses recursion to find a path to the goal position using DFS
// returns true if goal position is found and false if not
bool Maze::findPathDFS(short x, short y, Direction d) {
    if (x < 0 || x > columns - 1 || y < 0 || y > rows - 1) {
        // returns false if either the x or y value are outside the maze
        return false;
    } else if (mazeChars[x][y] == '#') {
        // returns false if there is an obstacle at the given x and y postion
        return false;
    } else if (marked[x][y]) {
        // return false if the given x and y position has already been marked
        return false;
    }

    // the direction from the path before is saved in the pathBefore vector
    pathBefore[x][y].clear();
    pathBefore[x][y].push_back(d);
    // the x and y position is marked as part of the solution path
    marked[x][y] = true;
    // current is set to the current x and y position
    current.x = x;
    current.y = y;
    // the maze is drawn
    drawMaze();
    // the variable speed determines how quickly the maze will be drawn
    al_rest(speed);

    if (mazeChars[x][y] == 'G')
        // returns true if the goal position has been found
        return true;

    // method calls itself for all surrounding coordinates
    if (findPathDFS(x, y - 1, UP))
        return true;
    if (findPathDFS(x + 1, y, RIGHT))
        return true;
    if (findPathDFS(x, y + 1, DOWN))
        return true;
    if (findPathDFS(x - 1, y, LEFT))
        return true;

    // unmarks x and y position from the solution path
    marked[x][y] = false;
    return false;
}

/// Method Prototypes - Algorithm: BFS
// recieves no parameters
// uses recursion to find a path to the goal position using BFS
// returns true if goal position is found and false if not
bool Maze::findPathBFS() {
    if (nodeQueue.empty()) {
        // if the node queue is empty, method returns false since goal position has not been found
        // rather than traversing every node, this method uses BFS but stops once the goal position has been found
        return false;
    }

    // current is set the the top node, and the node is then popped
    current = nodeQueue.front();
    nodeQueue.pop();
    // the current x and y position is marked as part of the solution path
    marked[current.x][current.y] = true;
    // the maze is drawn
    drawMaze();
    // the variable speed determines how quickly the maze will be drawn
    al_rest(speed);

    // calls methods update queue based on all surrounding coordinates
    if (updateQueueBFS(current.x, current.y - 1, UP) || updateQueueBFS(current.x + 1, current.y, RIGHT) || updateQueueBFS(current.x, current.y + 1, DOWN) || updateQueueBFS(current.x - 1, current.y, LEFT)) {
        // if the goal position has been found, method is called to locate the shortest path, and true is returned
        drawColouredPath(goal.x, goal.y);
        return true;
    }
    // BFS algorithm is adapted to use recursion, so method calls itself
    // alternatively, the method could use a while loop
    return findPathBFS();
}
// recieves an x value, a y value, and a direction as parameters
// updates queue based on given coordinates
bool Maze::updateQueueBFS(short x, short y, Direction d) {
    if (x < 0 || x > columns - 1 || y < 0 || y > rows - 1) {
        // returns false if either the x or y value are outside the maze
        return false;
    } else if (mazeChars[x][y] == '#') {
        // returns false if there is an obstacle at the given x and y postion
        return false;
    } else if (visited[x][y] || marked[x][y]) {
        // return false if the given x and y position has already been marked or visited (added to the queue)
        return false;
    } else {
        // x and y position is added to the queue
        visited[x][y] = true;
        Coordinates node{x = x, y = y};
        nodeQueue.push(node);
        // the direction from the path before is saved in the pathBefore vector
        pathBefore[x][y].clear();
        pathBefore[x][y].push_back(d);
        if (mazeChars[x][y] == 'G') {
            // if the goal position has been found, the position is marked as true
            current = node;
            marked[x][y] = true;
            // the maze is drawn, and true is returned
            drawMaze();
            return true;
        }
    }
    return false;
}

/// Method Prototypes - Algorithm: Dijkstra
// recieves no parameters
// uses recursion to find a path to the goal position using Dijkstra's Algorithm
bool Maze::findPathDijkstra() {
    // sets lowest path cost to INT_MAX (the maximum value an integer can hold)
    int lowestPathCost = INT_MAX;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (pathCost[j][i] < lowestPathCost && !marked[j][i]) {
                // updates lowest path cost if a lower path cost to a point (that has not been marked) is found
                lowestPathCost = pathCost[j][i];
                // current is set to the coordinates of such point
                current.x = j;
                current.y = i;
            }
        }
    }

    // the current x and y position is marked as part of the solution path
    marked[current.x][current.y] = true;
    // the maze is drawn
    drawMaze();
    // the variable speed determines how quickly the maze will be drawn
    al_rest(speed);

    if (mazeChars[current.x][current.y] == 'G') {
        // if the goal position has been found, method is called to locate the shortest path, and true is returned
        drawColouredPath(goal.x, goal.y);
        return true;
    } else {
        // calls methods to update path cost based on all surrounding coordinates
        updatePCDijkstra(pathCost[current.x][current.y], current.x, current.y - 1, UP);
        updatePCDijkstra(pathCost[current.x][current.y], current.x + 1, current.y, RIGHT);
        updatePCDijkstra(pathCost[current.x][current.y], current.x, current.y + 1, DOWN);
        updatePCDijkstra(pathCost[current.x][current.y], current.x - 1, current.y, LEFT);
        // Dijkstra's algorithm is adapted to use recursion, so method calls itself
        // alternatively, the method could use a while loop
        return findPathDijkstra();
    }
}
// recieves current path cost, an x value, a y value, and a direction as parameters
// updates path cost for given coordinates
void Maze::updatePCDijkstra(int p, short x, short y, Direction d) {
    if (x < 0 || x > columns - 1 || y < 0 || y > rows - 1) {
        // returns if either the x or y value are outside the maze
        return;
    } else if (mazeChars[x][y] == '#') {
        // returns if there is an obstacle at the given x and y postion
        return;
    }
    if (p + 1 < pathCost[x][y]) {
        // if the new path cost to the given point is less than the previous path cost, the point's path cost is updated
        pathCost[x][y] = p + 1;
        // the direction from the path before is saved in the pathBefore vector
        pathBefore[x][y].clear();
        pathBefore[x][y].push_back(d);
    }
}

/// Method Prototypes - Algorithm: A*
// recieves no parameters
// uses recursion to find a path to the goal position using A* Algorithm
bool Maze::findPathAStar() {
    // sets lowest path cost to INT_MAX (the maximum value an integer can hold)
    int lowestPathCost = INT_MAX;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (pathCost[j][i] < lowestPathCost && !marked[j][i] || (pathCost[j][i] == lowestPathCost && gCost[j][i] < gCost[current.x][current.y] && !marked[j][i])) {
                // updates lowest path cost if a lower path cost to a point (that has not been marked) is found
                // if coordinates contain the same path cost, the coordinate with the lowwe g cost is selected
                lowestPathCost = pathCost[j][i];
                // current is set to the coordinates of such point
                current.x = j;
                current.y = i;
            }
        }
    }

    // the current x and y position is marked as part of the solution path
    marked[current.x][current.y] = true;
    // the maze is drawn
    drawMaze();
    // the variable speed determines how quickly the maze will be drawn
    al_rest(speed);

    if (mazeChars[current.x][current.y] == 'G') {
        // if the goal position has been found, method is called to locate the shortest path, and true is returned
        drawColouredPath(goal.x, goal.y);
        return true;
    } else {
        // calls methods to update path cost based on all surrounding coordinates
        updatePCAStar(gCost[current.x][current.y], current.x, current.y - 1, UP);
        updatePCAStar(gCost[current.x][current.y], current.x + 1, current.y, RIGHT);
        updatePCAStar(gCost[current.x][current.y], current.x, current.y + 1, DOWN);
        updatePCAStar(gCost[current.x][current.y], current.x - 1, current.y, LEFT);
        // A* algorithm is adapted to use recursion, so method calls itself
        // alternatively, the method could use a while loop
        return findPathAStar();
    }
}
// recieves current g cost, an x value, a y value, and a direction as parameters
// updates g cost and path cost for given coordinates
void Maze::updatePCAStar(int g, short x, short y, Direction d) {
    if (x < 0 || x > columns - 1 || y < 0 || y > rows - 1) {
        // returns if either the x or y value are outside the maze
        return;
    } else if (mazeChars[x][y] == '#') {
        // returns if there is an obstacle at the given x and y postion
        return;
    }
    // calls method to calculate h cost
    float h = hCostAStar(x, y);
    if (g + 1 + h < pathCost[x][y]) {
        // if the new path cost to the given point is less than the previous path cost, the point's path cost and g cost is updated
        gCost[x][y] = g + 1;
        pathCost[x][y] = gCost[x][y] + h;
        // the direction from the path before is saved in the pathBefore vector
        pathBefore[x][y].clear();
        pathBefore[x][y].push_back(d);
    }
}
// recieves an x value and a y value as parameters
// uses the distance formula to calculate the distance from these coordinates to the goal position
// returns this distance as a float
float Maze::hCostAStar(short x, short y) {
    return sqrt(pow((float(goal.x) - x), 2) + pow((float(goal.y) - y), 2));
}

/// Method Prototypes - Algorithm: Manual
// recieves no parameters
// creates path based on user's input (using the arrow keys)
bool Maze::findPathManual() {
    // current is set to the starting position
    current = start;

    // message is displayed to tell user to use the arrow keys
    al_draw_filled_rounded_rectangle(160, 365, 540, 395, 3, 3, BLACK);
    al_draw_text(basic_font, PINK, 350, 365, ALLEGRO_ALIGN_CENTRE, "Use Arrow Keys to Solve the Maze");
    al_flip_display();

    ALLEGRO_EVENT ev;

    while (true) {
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            // the updatePathManual method is called to update the user's path based on their input
            // returns true if the goal position has been found
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                if (updatePathManual(current.x, current.y - 1, UP))
                    return true;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                if (updatePathManual(current.x + 1, current.y, RIGHT))
                    return true;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                if (updatePathManual(current.x, current.y + 1, DOWN))
                    return true;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                if (updatePathManual(current.x - 1, current.y, LEFT))
                    return true;
            }
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // returns false if user closes the allegro display
            return false;
        }
    }
}
// recieves an x value, a y value, and a direction as parameters
// updates path based on the given coordinates
bool Maze::updatePathManual(short x, short y, Direction d) {
    if (x < 0 || x > columns - 1 || y < 0 || y > rows - 1) {
        // returns false if either the x or y value are outside the maze
        return false;
    } else if (mazeChars[x][y] == '#') {
        // returns if there is an obstacle at the given x and y postion
        return false;
    } else {
        // the x and y position is marked as part of the solution path
        marked[x][y] = true;
        // current is set to the current x and y position
        current.x = x;
        current.y = y;
        // the direction from the path before is saved in the pathBefore vector
        pathBefore[x][y].push_back(d);
        drawMaze();
        if (mazeChars[x][y] == 'G') {
            // returns true if the goal position has been found
            return true;
        }
    }
    return false;
}

/// Method Prototypes - Find Shortest Path
// method recieves an x and y position
// uses recursion to backtrack either BFS, Dijkstra, or A* and find the shortest path
void Maze::drawColouredPath(short x, short y) {
    // boolean for coloured at the given x and y position is set to true
    coloured[x][y] = true;

    if (x == start.x && y == start.y) {
        // method returns if x and y position matches the starting position
        return;
    }

    // method calls itself for selected coordinates based on the direction from the path before (saved in the pathBefore vector)
    if (count(pathBefore[x][y].begin(), pathBefore[x][y].end(), UP)) {
        drawColouredPath(x, y + 1);
    } else if (count(pathBefore[x][y].begin(), pathBefore[x][y].end(), RIGHT)) {
        drawColouredPath(x - 1, y);
    } else if (count(pathBefore[x][y].begin(), pathBefore[x][y].end(), DOWN)) {
        drawColouredPath(x, y - 1);
    } else if (count(pathBefore[x][y].begin(), pathBefore[x][y].end(), LEFT)) {
        drawColouredPath(x + 1, y);
    }
}
