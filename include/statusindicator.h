#ifndef _STATUSINDICATOR_H
#define _STATUSINDICATOR_H

#include <Arduino.h>

enum StatusIndicatorState {
    TIMER,  // shows 's' to indicate timer seconds
    LEVEL,  // shows '%' to indicate fill-level percentage
    CLEAN,  // future feature, shows a cleaning hint after certain number of shots
    ERROR   // shows '?' to indicate failure of reading fill-level
};

/**
 * 
 * StatusIndicator class manages the small status symbol at the bottom of the screen.
 * 
 */
class StatusIndicator {
private:
    // If status indicator is visible
    bool _hidden;
    // Current state of status indicator
    StatusIndicatorState _state;
    // Center coordinates of status indicator
    uint8_t _xCenter, _yCenter;

    /**
     * Draws the respective status symbol according to _state in the given color
     */
    void drawSymbol(uint16_t color);

public:
    StatusIndicator();

    /**
     * Hides the status indicator by redrawing the current symbol in black
     */
    void hide();

    /**
     * Updates the status indicator with the newStatus and the given color
     * Removes the old symbol before redrawing.
     */
    void showStatus(StatusIndicatorState newStatus, uint16_t color);
};

#endif