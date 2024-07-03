#include <ringindicator.h>
#include <graphics.h>
#include <Arduino.h>
#include <math.h>
#include <serialutils.h>
#include <config.h>
#include <display.h>

RingIndicator::RingIndicator()
    : _xCenter(RING_CENTER_X), _yCenter(RING_CENTER_Y), _radius(RING_RADIUS), _width(RING_WIDTH), _color(RING_COLOR_OK) {

    // Default to full circle
    _radMin = 0.0;
    _radMax = 2*PI;
    _hidden = true;

    _radValue = _radMin;
    _radOffset = 0.0;
}

void RingIndicator::setLimits(float radMin, float radMax) {
    if(radMin > radMax) {
        sout.warn() <= "setLimits: Refusing to set limits with min > max";
        return;
    }

    hide();
    _radMin = radMin;
    _radMax = radMax;
    _radValue = _radMin;
};

void RingIndicator::drawRing(uint16_t color, uint8_t width) {
    Graphics::drawRoundedArc(_xCenter, _yCenter, _radius, 
        _radMin+_radOffset, _radOffset+_radValue, width, color);

    _hidden = false;
}

void RingIndicator::hide() {
    // Remove the current arc by redrawing in black.
    // Note: the width is increased to accomodate for the rounded arc ends.
    drawRing(BLACK, _width+2);
    _radValue = _radMin;
    _hidden = true;
}

void RingIndicator::updatePrec(float percValue, uint16_t color) {
    // Clip to [0, 100]
    if(percValue > 100) {
        sout.warn() <= "RingIndicator::updatePrec: percValue > 100. Clipping to 100";
        percValue = 100;
    }
    if(percValue < 0) {
        sout.warn() <= "RingIndicator::updatePrec: percValue < 0. Clipping to 0";
        percValue = 0;
    }

    // Convert to rad in [_radMin _radMax] interval
    float radValue = (percValue/100)*(_radMax - _radMin) + _radMin;

    // Perform update based on radiant value
    update(radValue, color);
}

void RingIndicator::update(float radValue, uint16_t color) {
    // Clip to [_minRad, _maxRad]
    if(radValue < _radMin) {
        sout.warn() <= "RingIndicator::update: radValue < _radMin. Clipping value to _radMin.";
        radValue = _radMin;
    }
    if (radValue > _radMax) {
        sout.warn() <= "RingIndicator::update: radValue > _radMax. Clipping value to _radMax.";
        radValue = _radMax;
    }

    if(_hidden) {
        // Ring was hidden, need to draw complete ring.
        _color = color;
        _radValue = radValue;
        drawRing(_color, _width);
        _hidden = false;
    } else {

        if(color != _color) {
            // Color changed. Need complete redraw
            
            // Remove old ring
            hide();

            // Update ring state
            _color = color;
            _radValue = radValue;

            // Draw new ring
            drawRing(_color, _width);
        } else {
            if(radValue > _radValue) {
                // Extend arc. In this case, start corners don't need to be rounded
                float radArcStart = _radOffset+_radValue;
                float radArcEnd = _radOffset+radValue;
                uint8_t x_end = _xCenter + sinf(radArcEnd)*_radius;
                uint8_t y_end = _yCenter + cosf(radArcEnd)*_radius;

                // Draw arc extension
                Graphics::drawArc(_xCenter, _yCenter, _radius, radArcStart, radArcEnd, _width, _color);
                // Round end of extension by drawing a circle
                Graphics::drawFullCircle(x_end, y_end, _width/2, _color);
            } else {
                // Retract arc. In this case, draw a black arc to remove the excess.
                // The arc is slightly offset to keep the rounding
                float radArcStart = _radOffset+radValue+0.1;
                float radArcEnd = _radOffset+_radValue+0.1;

                uint8_t x_end = _xCenter + sinf( _radOffset+radValue)*_radius;
                uint8_t y_end = _yCenter + cosf( _radOffset+radValue)*_radius;

                // Remove old portion of arc
                if(radArcStart < radArcEnd) {
                    Graphics::drawArc(_xCenter, _yCenter, _radius, radArcStart, radArcEnd, _width+2, BLACK);
                }
                // Recreate rounding by cutting with black arc
                Graphics::drawArc(x_end, y_end, _width/2 + 5, radArcStart, radArcStart+PI, 8, BLACK);
            }
            _radValue = radValue;
        }

    }

}