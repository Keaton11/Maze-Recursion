/*  Name: Keaton Lee
 *  Date: June 12, 2021
 *  Title: Maze Recusion
 *
 *  Description: Contains functions used to display the options menu and the maze menu
*/

// imports header files
#include "header.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *title_font;
extern ALLEGRO_FONT *basic_font;
extern ALLEGRO_TIMER *timer;

/// Play Function

void playMenu() {
// function recieves no parameters
// options menu (containing maze, speed, and algorithm selection) is displayed
// calls function to draw maze once user has made a selection and clicked next
// returns once player clicks exit or closes the allegro display

    // variables are declared
    short selectedMaze = -1;        // contains value corresponding to the user's maze selection
    short selectedAlgorithm = -1;   // contains value corresponding to the user's algorithm selection
    float speedX = 350;             // contains value corresponding to the selected speed
    bool updateSpeed;               // used to determine whether speed should be updated

    // next, exit, and refresh buttons are declared
    Button nextButton("Next", 150, 50, 275, 650, GREEN, BLACK);
    Button exitButton("Exit", 150, 50, 75, 650, IRIS, BLACK);
    Button refreshButton("Refresh", 150, 50, 475, 650, IRIS, BLACK);

    // Button vectors are created for the maze and algorithm selections
    vector<Button> optionsMaze;
    vector<Button> optionsAlgorithm;

    // for loop is used to postion the maze and algorithm buttons
    for (int i = 0; i < 5; i++) {
        // maze buttons are added to the optionsMaze vector
        Button mazeButton("", 150, 50, 75, 170 + i * 80, IRIS, BLACK);
        optionsMaze.push_back(mazeButton);
        // algorithm buttons are added to the optionsAlgorithm vector
        Button algorithmButton("", 150, 50, 475, 170 + i * 80, IRIS, BLACK);
        optionsAlgorithm.push_back(algorithmButton);
    }

    // text is set for the maze buttons
    optionsMaze[0].setText("File Input");
    optionsMaze[1].setText("5 x 5");
    optionsMaze[2].setText("10 x 10");
    optionsMaze[3].setText("15 x 15");
    optionsMaze[4].setText("20 x 20");

    // text is set for the algorithm buttons
    optionsAlgorithm[0].setText("DFS");
    optionsAlgorithm[1].setText("BFS");
    optionsAlgorithm[2].setText("Dijkstra");
    optionsAlgorithm[3].setText("A Star");
    optionsAlgorithm[4].setText("Manual");

    ALLEGRO_EVENT ev;

    while (true) {
        // timer event source is unregistered
        al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

        // background colour and title are added to the allegro display
        al_clear_to_color(BLACK);
        al_draw_text(title_font, IRIS, 350, 30, ALLEGRO_ALIGN_CENTRE, "Maze Assignment");

        // sub titles for maze, speed, and algorithm are added to the allegro display
        al_draw_text(basic_font, LAVENDER, 150, 120, ALLEGRO_ALIGN_CENTRE, "Select A Maze");
        al_draw_text(basic_font, LAVENDER, 550, 120, ALLEGRO_ALIGN_CENTRE, "Select An Algorithm");
        al_draw_text(basic_font, LAVENDER, 350, 300, ALLEGRO_ALIGN_CENTRE, "Speed");

        // speed bar is added to the allegro display
        al_draw_filled_rounded_rectangle(260, 350, 440, 370, 3, 3, IRIS);
        al_draw_rounded_rectangle(260, 350, 440, 370, 3, 3, LAVENDER, 2);
        al_draw_filled_triangle(speedX - 15, 340, speedX + 15, 340, speedX, 365.9808, GREEN);

        // instructions message is added to the allegro display
        al_draw_text(basic_font, LAVENDER, 350, 580, ALLEGRO_ALIGN_CENTRE, "Select a Maze and an Algorithm and then click Next!");

        if (selectedAlgorithm == -1 || selectedMaze == -1) {
            // if a maze and algorithm have not been selected, the next button is set to grey with hover set to false
            nextButton.setColour(GREY);
            nextButton.setHover(false);
        } else {
            // if a maze and algorithm have been selected, the next button is set to green with hover set to true
            nextButton.setColour(GREEN);
            nextButton.setHover(true);
        }

        // next, exit, and refresh buttons are drawn (added to the allegro display)
        nextButton.draw(ev);
        exitButton.draw(ev);
        refreshButton.draw(ev);

        // maze and algorithm buttons are drawn (added to the allegro display)
        for (int i = 0; i < 5; i++) {
            optionsMaze[i].draw(ev);
            optionsAlgorithm[i].draw(ev);
        }

        // allegro display is updated
        al_flip_display();

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // if mouse button is down, updateSpeed is set to true so speed can be updated
            updateSpeed = true;
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // if mouse button is up, updateSpeed is set to false so speed cannot be updated
            updateSpeed = false;
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (updateSpeed) {
                if (ev.mouse.x > speedX - 15 && ev.mouse.x < speedX + 15 && ev.mouse.y > 340 && ev.mouse.y < 365.9808) {
                    // if the user is clicking the speed triangle, the triangle's x position while update to match the users
                    speedX = ev.mouse.x;
                    // the x value for speed must remain within the length of the speed bar (275 <= x, x <= 425)
                    if (speedX < 275) {
                        speedX = 275;
                    } else if (speedX > 425) {
                        speedX = 425;
                    }
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            if (optionsMaze[i].checkClicked(ev)) {
                // if a maze button is clicked, the updateOptionsMenu function is called to update the maze buttons
                selectedMaze = i;
                updateOptionsMenu(false, optionsMaze, i);
                // the timer event source is registered to update the buttons
                al_register_event_source(event_queue, al_get_timer_event_source(timer));
            }
            if (optionsAlgorithm[i].checkClicked(ev)) {
                // if an algorithm button is clicked, the updateOptionsMenu function is called to update the algorithm buttons
                selectedAlgorithm = i;
                updateOptionsMenu(false, optionsAlgorithm, i);
                // the timer event source is registered to update the buttons
                al_register_event_source(event_queue, al_get_timer_event_source(timer));
            }
        }

        if (nextButton.checkClicked(ev)) {
            // if the next button is clicked, the maze menu function is called
            if (mazeMenu(abs((speedX - 274) / 15 - 10.1) * 0.05, selectedMaze, selectedAlgorithm, optionsMaze[selectedMaze].getText(), optionsAlgorithm[selectedAlgorithm].getText())) {
                // if the maze menu function returns true, variables and options are reset so the user can select new options
                speedX = 350;
                selectedMaze = -1;
                selectedAlgorithm = -1;
                updateOptionsMenu(true, optionsMaze, 0);
                updateOptionsMenu(true, optionsAlgorithm, 0);
                // the timer event source is registered to update the allegro display
                al_register_event_source(event_queue, al_get_timer_event_source(timer));
            } else {
                // function returns if maze menu function returns false
                return;
            }
        } else if (refreshButton.checkClicked(ev)) {
            // if refresh button is clicked, variables and options are reset so the user can select new options
            speedX = 350;
            selectedMaze = -1;
            selectedAlgorithm = -1;
            updateOptionsMenu(true, optionsMaze, 0);
            updateOptionsMenu(true, optionsAlgorithm, 0);
            // the timer event source is registered to update the allegro display
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
        } else if (exitButton.checkClicked(ev) || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // function returns if the exit button is clicked or the allegro display is closed
            return;
        }

        al_wait_for_event(event_queue, &ev);
    }
}

/// Update Options Menu Function

void updateOptionsMenu(bool refresh, vector<Button> &buttonVector, int clicked) {
// function recieves a boolean for refresh, a button vector, and an integer corresponding to the button clicked as parameters
// updates button vector based on boolean for refresh and clicked integer
// returns button vector via pass by reference

    for (int i = 0; i < buttonVector.size(); i++) {
        if (refresh) {
            // if refresh is set to true, all buttons are set to IRIS, and hover set to true
            buttonVector[i].setColour(IRIS);
            buttonVector[i].setHover(true);
        } else {
            // if refresh is not set to true, all buttons are set to GREY except the clicked button (which is set to GREEN)
            // hover is set to false so buttons cannot be clicked again
            if (i != clicked) {
                buttonVector[i].setColour(GREY);
            } else {
                buttonVector[i].setColour(GREEN);
            }
            buttonVector[i].setHover(false);
        }
    }
}

/// Maze Menu Function

bool mazeMenu(float speed, short selectedMaze, short selectedAlgorithm, string mazeText, string algorithmText) {
// function recieves values corresponding to the selected maze, speed, and algorithm as parameters
// displays maze and algorithm accordingly
// returns false if the exit button is clicked or the allegro display is closed
// returns true if the play again button is clicked

    // background colour, title, and descriptions are added to the allegro display
    al_clear_to_color(BLACK);
    al_draw_text(title_font, IRIS, 350, 30, ALLEGRO_ALIGN_CENTRE, "Maze Assignment");
    al_draw_textf(basic_font, IRIS, 75, 675, ALLEGRO_ALIGN_LEFT, "Maze: %s", mazeText.c_str());
    al_draw_textf(basic_font, IRIS, 625, 675, ALLEGRO_ALIGN_RIGHT, "Algorithm: %s", algorithmText.c_str());

    // maze object is created (with the selected speed)
    Maze maze1(speed);

    if (selectedMaze == 0) {
        // if the file input option was selected, the maze is set based on the file "maze1.txt"
        maze1.fillMazeFile("maze1.txt");
    } else {
        // if a randomly generated maze was selected, the maze is updated based on the selected dimensions
        maze1.fillMazeGenerate(selectedMaze * 5);
    }

    // method is called to draw the maze
    maze1.drawMaze();

    // method is called to draw the selected alogrithm
    maze1.findPath(selectedAlgorithm);

    // exit and play again buttons are declared
    Button exitButton("Exit", 150, 50, 75, 670, IRIS, BLACK);
    Button playAgainButton("Play Again", 150, 50, 475, 670, IRIS, BLACK);

    ALLEGRO_EVENT ev;

    while (true) {
        // background colour is added to the allegro display
        al_clear_to_color(BLACK);
        // title is updated to reflect that the maze has been completed
        al_draw_text(title_font, IRIS, 350, 30, ALLEGRO_ALIGN_CENTRE, "Maze Completed");

        // exit and play again buttons are drawn (added to the allegro display)
        exitButton.draw(ev);
        playAgainButton.draw(ev);

        // maze (containing path) is redrawn
        maze1.drawMaze();

        al_wait_for_event(event_queue, &ev);

        if (playAgainButton.checkClicked(ev)) {
            // function returns true if the play again button is clicked
            return true;
        } else if (exitButton.checkClicked(ev) || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // function returns false if the exit button is clicked or the allegro display is closed
            return false;
        }
    }
}
