#include <stdlib.h>
#include <graphics.h>
#include <math.h>
#include <Arduino.h>
#include <serialutils.h>
#include <display.h>
#include <Roboto_Light50pt7b.h>

namespace Graphics {

    void drawArc(uint8_t xCenter, uint8_t yCenter, uint8_t radius,
                float radStart, float radEnd, uint8_t width, uint16_t color) {

        // Sanity checks
        if(radStart >= radEnd) {
            sout.err() <= "drawArc called with radStart < radEnd";
            return;
        };

        if(radius < 0) {
            sout.err() <= "drawArc called with radius < 0";
            return;
        }

        if(width < 1) {
            sout.err() <= "drawArc called with width < 1";
            return;
        }

        // Keep track of previously painted pixel to skip some double paintings
        int prevX, prevY = -1;

        float radiusCur = max(((float)radius) - (((float)width)/2), 1);
        float radiusEnd = ((float)radius) + (((float)width)/2);

        while(radiusCur < radiusEnd) {

            // Calculate angular resolution based on worst-case angular distance
            float angularRes = asinf(1/((float) radiusCur)) * ARC_DISTANCE_RES;

            // Reset current radiants
            float radCur = radStart;
            
            while(radCur < radEnd) {

                int x = xCenter + sinf(radCur)*radiusCur;
                int y = yCenter + cosf(radCur)*radiusCur;

                // Only draw pixel if it
                // 1. is inside display boundaries
                // 2. hasn't been painted in previous loop
                if(x > 0 && y > 0 && x < LCD_HEIGHT && y < LCD_WIDTH && !(prevX == x && prevY == y)) {
                    Display::setPixel(x, y, color);
                }

                prevX = x;
                prevY = y;

                radCur += angularRes;
            }

            radiusCur += ARC_DISTANCE_RES;

        }

    }

    void drawFullCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint16_t color) {
        // Draw a full circle by drawing a full arc with enough width.
        float radStart = 0;
        float radEnd = 2*PI;
        drawArc(xCenter, yCenter, 0, radStart, radEnd, 2*radius, color);

    }

    void drawRoundedArc(uint8_t xCenter, uint8_t yCenter, uint8_t radius,
                    float radStart, float radEnd, uint8_t width, uint16_t color) {

        // Position of circle at start
        int x_start = xCenter + sinf(radStart)*radius;
        int y_start = yCenter + cosf(radStart)*radius;

        // Position of circle at end
        int x_end = xCenter + sinf(radEnd)*radius;
        int y_end = yCenter + cosf(radEnd)*radius;
        
        // Draw circle at arc start for rounded corners
        drawFullCircle(x_start, y_start, width/2, color);
        // Draw actual arc
        drawArc(xCenter, yCenter, radius, radStart, radEnd, width, color);
        // Draw circle at arc end for rounded corners
        drawFullCircle(x_end, y_end, width/2, color);
    }

    void drawChar(uint8_t x, uint8_t y, unsigned char c, uint16_t color, 
                const GFXfont *font, const GFXglyph *glyph, const uint8_t *bitmap) {
        // Determine character glyph index and call drawbyindex function
        c -= (uint8_t)(font->first);
        drawCharByIndex(x, y, c, color, glyph, bitmap);
    }

    void drawCharByIndex(uint8_t x, uint8_t y, uint8_t glyphIndex, uint16_t color, 
                const GFXglyph *glyph, const uint8_t *bitmap) {

        // Get drawing parameters for glyph
        uint16_t bo = glyph[glyphIndex].bitmapOffset;
        uint8_t w = glyph[glyphIndex].width;
        uint8_t h = glyph[glyphIndex].height;
        int8_t xo = glyph[glyphIndex].xOffset;
        int8_t yo = glyph[glyphIndex].yOffset;
        uint8_t xx, yy, bits = 0, bit = 0;
        int16_t xo16 = 0, yo16 = 0;

        // Iterate through columns and rows of glyph table to draw character
        for (yy = 0; yy < h; yy++) {
            for (xx = 0; xx < w; xx++) {
                if (!(bit++ & 7)) {
                    bits = bitmap[bo++];
                }
                if (bits & 0x80) {
                    Display::setPixel(x + xo + xx, y + yo + yy, color);
                }
                bits <<= 1;
            }
        }
    }

}