#ifndef _GREETINGDISPLAY_H
#define _GREETINGDISPLAY_H

#include <pico/stdlib.h>

/**
 * 
 * GreetingDisplay class to display random coffee-related greeting messages
 * in the center of the display
 * 
 */
class GreetingDisplay {
private:
    // Center position for greeting display
    uint8_t _xCenter;
    uint8_t _yCenter;
    
    // Track if greeting is currently shown
    bool _hidden;
    
    // Current greeting index
    uint8_t _currentGreeting;

    /**
     * Array of coffee-related greeting messages
     */
    static const char* greetings[];
    static const uint8_t numGreetings;

    /**
     * Draws the current greeting message
     */
    void drawGreeting(uint16_t color);

public:
    GreetingDisplay();

    /**
     * Shows a random greeting message in the given color
     */
    void showRandom(uint16_t color);

    /**
     * Hides the current greeting by redrawing it in black
     */
    void hide();
};

#endif

