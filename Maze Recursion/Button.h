/*  Name: Keaton Lee
 *  Date: June 12, 2021
 *  Title: Maze Recusion
 *
 *  Description: Protoype for Button class
*/

// checks if header file was defined to avoid double inclusions
#ifndef BUTTON_H
#define BUTTON_H

// imports header files
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <string>
#include <vector>

using namespace std;

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *basic_font;

class Button {
    public:

        /// Constructors
        Button(string t, int l, int w, int x, int y, ALLEGRO_COLOR c, ALLEGRO_COLOR tc);
        Button();       // default constructor

        /// Destructor
        ~Button();

        /// Setters (Mutators)
        void setText(string t);
        void setColour(ALLEGRO_COLOR c);
        void setHover(bool h);

        /// Getters (Accessors)
        string getText();

        /// Other Method Prototypes
        void draw(ALLEGRO_EVENT &ev);
        bool checkClicked(ALLEGRO_EVENT &ev);

        static ALLEGRO_COLOR findHoverColour(ALLEGRO_COLOR c);

    private:

        /// Private Variables
        int length;     // length of the button
        int width;      // width of the button

        int xPos;       // top left x position of button
        int yPos;       // top left y position of button

        string text;    // text written on button

        bool hover;     // boolean determines whether button should respond to hovering

        ALLEGRO_COLOR colour;       // colour of button
        ALLEGRO_COLOR textColour;   // colout of text on button
        ALLEGRO_COLOR hoverColour;  // colour of button when user is hovering over it
};

#endif // BUTTON_H
