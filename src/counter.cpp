#include <counter.h>
#include <math.h>
#include <graphics.h>
#include <Roboto_Light50pt7b.h>
#include <Arduino.h>
#include <serialutils.h>
#include <config.h>
#include <display.h>

Counter::Counter() :
    _xCenter(COUNTER_CENTER_X), _yCenter(COUNTER_CENTER_Y), _nDigits(0) {
    
    // Get index of first glyph
    uint8_t fontGlyphOffset = (uint8_t)(Roboto_Light50pt7b.first);

    // Iterate through possible digits to:
    // 1. create a glyph index lookup table
    // 2. find maximum digit width and height
    uint8_t max_height, max_width, curr_height, curr_width = 0;

    for(int i=0; i<10; i++) {

        // Glyph for character '0' has an index offset of 48
        _glyphLookup[i] = (char)(i + 48) - fontGlyphOffset;

        // Get glyph width and height
        curr_height = Roboto_Light50pt7bGlyphs[_glyphLookup[i]].height;
        curr_width = Roboto_Light50pt7bGlyphs[_glyphLookup[i]].width;

        // Update maximum values
        if(curr_height > max_height) max_height = curr_height;
        if(curr_width > max_width) max_width = curr_width;
    }

    // Store largest digit dimensions. These will be used for all digits.
    _digitHeight = max_height;
    _digitWidth = max_width;

}


uint8_t Counter::getNumberOfDigits(uint8_t value) {
    // Use logarithm of base 10 to determine number of digits in value.
    // Special case is value=0, for which 1 is returned
    return value == 0 ? 1 : floor(log10(value)) + 1;
};

uint8_t Counter::getDigits(uint8_t value, uint8_t* buffer) {
    uint8_t remainder = value;
    uint8_t nDigits = getNumberOfDigits(value);

    // Loop goes from nDigits-1...0 to reverse the order in which
    // digits are stored in buffer. 
    // Hence, buffer[0] gives the digit of the largest decimal place
    // This makes it easier to draw the digit later on.
    for(int i=nDigits-1; i > -1; i--) {
        // Get digits for current decimal
        buffer[i] = floor(remainder / pow(10, i));
        // Update remainder for next loop
        remainder -= buffer[i] * pow(10, i);
    }

    return nDigits;
};

void Counter::getDigitLowerLeft(uint8_t nDigits, uint8_t digitIndex, uint8_t& xLowerLeft, uint8_t& yLowerLeft) {

    // Calculate total width
    uint8_t total_width = nDigits*_digitWidth + (nDigits - 1)*_digitSpacing;

    // Calculate lower left corner of desired digit index
    xLowerLeft = _xCenter - total_width/2 + (nDigits - 1 - digitIndex)*(_digitWidth + _digitSpacing);
    yLowerLeft = _yCenter + _digitHeight/2;

}

void Counter::drawDigit(uint8_t digitIndex, uint16_t color) {
    // Get lower left coordinate for desired digit
    uint8_t xLowerLeft, yLowerLeft = 0;
    getDigitLowerLeft(_nDigits, digitIndex, xLowerLeft, yLowerLeft);
    // Draw digit at given position
    Graphics::drawCharByIndex(xLowerLeft, yLowerLeft, _glyphLookup[_digits[digitIndex]], color, Roboto_Light50pt7bGlyphs, Roboto_Light50pt7bBitmaps);
}

void Counter::hide() {
    // Redraw all current digits in black to remove the counter
    for(int i=0; i<_nDigits; i++) {
        drawDigit(i, BLACK);
    }
    // Reset number of shown digits
    _nDigits = 0;
}

void Counter::update(uint8_t value, uint16_t color) {
    // Get number of new digits and store new digits into buffer
    uint8_t nDigits_new = getDigits(value, _digitsNew);

    // Check if the number of digits changed
    if(nDigits_new != _nDigits) {
        // Complete counter update required
        hide();

        _nDigits = nDigits_new;

        for(int i=0; i<_nDigits; i++) {
            _digits[i] = _digitsNew[i];
            drawDigit(i, color);
        }
    } else {
        // Only update digits that changed
        for(int i=0; i<_nDigits; i++) {
            if(_digits[i] != _digitsNew[i]) {
                // Clear old digit
                drawDigit(i, BLACK);
                // Show new digit
                _digits[i] = _digitsNew[i];
                drawDigit(i, WHITE);
            }
        }
    }
}