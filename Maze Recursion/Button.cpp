/*  Name: Keaton Lee
 *  Date: June 12, 2021
 *  Title: Maze Recusion
 *
 *  Description: Holds the implementations of the methods associated with the
 *  Button class
*/

// includes header file
#include "Button.h"

/// Constructors
// parameterized constructor
Button::Button(string t, int l, int w, int x, int y, ALLEGRO_COLOR c, ALLEGRO_COLOR tc) {
    // variables are set based on parameter values
    text = t;
    length = l;
    width = w;
    xPos = x;
    yPos = y;
    colour = c;
    textColour = tc;

    hoverColour = findHoverColour(c);   // static method is called to set hover colour based on button colour
    hover = true;   // hover is set to true by default
}
// parameter-less constructor
Button::Button() {
}

/// Destructor
Button::~Button() {
}

/// Setters (Mutators)
// recieves a string (t) as parameters
// sets text (message that will appear on button)
void Button::setText(string t) {
    text = t;
}
// recieves a boolean (h) as parameters
// sets hover boolean (which determines whether button should respond to hovering)
void Button::setHover(bool h) {
    hover = h;
}
// recieves an allegro colour (c) as parameters
// sets colour of button
void Button::setColour(ALLEGRO_COLOR c) {
    colour = c;
}

/// Getters (Accessors)
// recieves no parameters
// returns text as a string
string Button::getText() {
    return text;
}

/// Other Method Implementations
// recieves an allegro event as parameters
// draws button (colour depends on whether user is hovering over it)
void Button::draw(ALLEGRO_EVENT &ev) {
    if (ev.mouse.x > xPos && ev.mouse.x < xPos + length && ev.mouse.y > yPos && ev.mouse.y < yPos + width && hover) {
        // if user is hovering over the button, the button is drawn in the hover colour
        // if hover is set to false, the button is not drawn in the hover colour
        al_draw_filled_rounded_rectangle(xPos - 2, yPos - 2, xPos + length + 2, yPos + width + 2, 10, 10, hoverColour);
    } else {
        // if user is not hovering over the button, the button is drawn in the button's colour
        al_draw_filled_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 10, 10, colour);
    }
    // text is drawn centered on button in the text colour
    al_draw_text(basic_font, textColour, xPos + length / 2, yPos + width / 2 - 16, ALLEGRO_ALIGN_CENTRE, text.c_str());
}
// recieves an allegro event as parameters
// determines whether the button has been clicked
// returns true if button was clicked and false if not
bool Button::checkClicked(ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (ev.mouse.x > xPos && ev.mouse.x < xPos + length && ev.mouse.y > yPos && ev.mouse.y < yPos + width && hover) {
            // returns true if button was clicked and hover is true
            return true;
        }
    }
    // returns false if button was not clicked or hover is false
    return false;
}
// recieves an allegro colour as parameters
// method uses rgb values from colour to produce a darker version of such colour
// returns darker version of initial colour (used for a button's hover colour)
ALLEGRO_COLOR Button::findHoverColour(ALLEGRO_COLOR c) {
    // vector is declared (used to hold RGB values of an allegro colour)
    vector<unsigned char> hColourRBG(3);

    // the rgb values of the colour from the parameters are unmapped into the unsigned char vector
    al_unmap_rgb(c, &hColourRBG[0], &hColourRBG[1], &hColourRBG[2]);

    for (int i = 0; i < 3; i++) {
        // each rgb value of the colour is decreased by 35 (to darken the colour)
        hColourRBG[i] -= 35;
        if (hColourRBG[i] < 0) {
            // if an rgb value is decreased to below 0, it is set to 0;
            hColourRBG[i] = 0;
        }
    }

    // a darker version of the initial colour (used for a button's hover colour) is returned
    return al_map_rgb(hColourRBG[0], hColourRBG[1], hColourRBG[2]);
}
