#ifndef _graphics_h
#define _graphics_h

#include <pico/stdlib.h>
#include <gfxfont.h>

//Angular drawing resolution
#define ARC_ANGULAR_RES 0.005
//Distance drawing resolution (< 1/sqrt(2))
#define ARC_DISTANCE_RES 0.7

/**
 * 
 * Graphics namespace includes drawing utilities to draw circles, arcs and characters
 * by drawing individual pixels
 * 
 */
namespace Graphics {
    /**
     * Draws an arc centered around (xCenter, yCenter) with given radius, thickness and color.
     * The arc is drawn from radStart to radEnd, both given in radiants.
     * 0 radiant is located at the bottom of the circle (respecting display rotation!).
     */
    void drawArc(uint8_t xCenter, uint8_t yCenter, uint8_t radius,
                float radStart, float radEnd, uint8_t width, uint16_t color);

    /**
     * Draws a full circle centered around (xCenter, yCenter) with given radius and color.
     */
    void drawFullCircle(uint8_t xCenter, uint8_t yCenter, 
                    uint8_t radius, uint16_t color);

    /**
     * Draws an arc centered around (xCenter, yCenter) with given radius, thickness and color.
     * The arc is drawn from radStart to radEnd, both given in radiants.
     * 0 radiant is located at the bottom of the circle (respecting display rotation!).
     * The start and end of the arc are "rounded" by drawing two additional circles respectively,
     * whose diameter matches the arc width.
     */
    void drawRoundedArc(uint8_t xCenter, uint8_t yCenter, uint8_t radius,
                    float radStart, float radEnd, uint8_t width, uint16_t color);

    /**
     * Draws the given character c with lower left corner (x, y) in the given color.
     * The font, glyph and bitmap specify the pixel table of the character.
     */
    void drawChar(uint8_t x, uint8_t y, unsigned char c, uint16_t color, 
                const GFXfont *font, const GFXglyph *glyph, const uint8_t *bitmap);

    /**
     * Draws a character based on its glyphIndex with lower left corner (x, y) in the given color.
     * The font, glyph and bitmap specify the pixel table of the character.
     */
    void drawCharByIndex(uint8_t x, uint8_t y, uint8_t glyphIndex, uint16_t color, 
                const GFXglyph *glyph, const uint8_t *bitmap);
}


/**
 * 
 * The following functions contain source-code from the AdafruitGFX library, 
 * whose license is included at the bottom of this file.
 *      - Graphics::drawChar
 *      - Graphics::drawCharByIndex
 * 
 Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#endif