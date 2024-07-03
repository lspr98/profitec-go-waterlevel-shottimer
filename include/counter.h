#ifndef _COUNTER_H
#define _COUNTER_H

#include <pico/stdlib.h>

/**
 * 
 * Counter class to display a number centered at a given position
 * 
 */
class Counter {
private:

    // Counter state and new counter state
    uint8_t _digits[3] {0};
    uint8_t _digitsNew[3] {0};

    // Look-up table to map numbers 0-9 to glyph indices
    uint8_t _glyphLookup[10];
    
    // Current number of digits displayed
    uint8_t _nDigits;

    // Counter position
    uint8_t _xCenter;
    uint8_t _yCenter;
    
    // Digit dimensions
    uint8_t _digitSpacing;
    uint8_t _digitWidth;
    uint8_t _digitHeight;

    /**
     * Returns the number of digits of the given value.
     */
    uint8_t getNumberOfDigits(uint8_t value);
    
    /**
     * Writes the individual digits of value into buffer. Returns the number of digits of the given value.
     */
    uint8_t getDigits(uint8_t value, uint8_t* buffer);

    /**
     * Calculates the bottom left pixel coordiantes and writes them into xLowerLeft and yLowerLeft.
     * The coordinates are valid for for the digit at index digitIndex given a number consisting
     * of nDigits digits. The coordinates are calculated in such a way, that the complete number
     * consisting of all digits is centered at (_xCenter, _yCenter)
     */
    void getDigitLowerLeft(uint8_t nDigits, uint8_t digitIndex, uint8_t& xLowerLeft, uint8_t& yLowerLeft);

    /**
     * Draws the digit with index digitIndex in the given color.
     */
    void drawDigit(uint8_t digitIndex, uint16_t color);

public:
    Counter();

    /**
     * Updates the counter with the new given value and color. 
     * Clears the old drawn numbers by redrawing them in black.
     */
    void update(uint8_t value, uint16_t color);

    /**
     * Hides the counter by redrawing numbers in black.
     * Resets _nDigits to zero.
     */
    void hide();

};


#endif