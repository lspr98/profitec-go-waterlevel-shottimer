#ifndef _RINGINDICATOR_H
#define _RINGINDICATOR_H

#include <pico/stdlib.h>

/**
 * 
 * RingIndicator class manages the state of the ring indicator including drawing it.
 * 
 */
class RingIndicator {

private:
    // If indicator is currently visible or not
    bool _hidden;

    // Current color of indicator
    uint16_t _color;

    // Center position, radius and width of arc
    uint8_t _xCenter, _yCenter, _radius, _width;

    // radValue:        Current value in radiants
    // radOffset:       Shift value in radiants to shift complete arc
    // radMin:          Minimum value of arc start
    // radMax:          Maximum value of arc end
    float _radValue, _radOffset, _radMin, _radMax;

    /**
     * Draws an arc with startValue = _radMin+_radOffset and endValue = _radValue+_radOffset
     * with the given color and width
     */
    void drawRing(uint16_t color, uint8_t width);

public:
    RingIndicator();

    /**
     * Update minimum and maximum radiant values of arc.
     */
    void setLimits(float radMin, float radMax);

    /**
     * Display a percentage value with the ring indicator.
     * The arc is drawn from _radMin to _radValue, 
     * where _radValue is:
     *      _radMin                                         if percValue <= 0
     *      _radMax                                         if percValue >= 100
     *      (percValue/100)*(_radMax - _radMin) + _radMin   otherwise
     */
    void updatePrec(float percValue, uint16_t color);

    /**
     * Display a radiant value with the ring indicator.
     * The arc is drawn from _radMin to _radValue, 
     * where _radValue is:
     *      _radMin                                         if radValue <= _radMin
     *      _radMax                                         if radValue >= _radMax
     *      radValue                                        otherwise
     */
    void update(float radValue, uint16_t color);

    /**
     * Hides the ring indicator and resets its value to _radMin.
     */
    void hide();

};

#endif