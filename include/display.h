#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Arduino.h>

// SPI configuration
#define PLL_SYS_KHZ (270 * 1000)
#define LCD_SPI_PORT    (spi1)
#define LCD_DC_PIN      (8)
#define LCD_CS_PIN      (9)
#define LCD_CLK_PIN     (10)
#define LCD_MOSI_PIN    (11)
#define LCD_RST_PIN     (12)
#define LCD_BL_PIN      (25)

// Display dimensions
#define LCD_HEIGHT 240
#define LCD_WIDTH 240

// The display seems to have some additional pixel rows towards the USB port.
// The extended height makes sure those are cleared when calling the Display::clear method
#define LCD_HEIGHT_EXT 245

// Scan direction of display
#define HORIZONTAL 0
#define VERTICAL   1

// Some colors
#define WHITE       0xFFFF
#define BLACK		0x0000
#define BLUE 		0x001F
#define BRED 		0XF81F
#define GRED 		0XFFE0
#define GBLUE		0X07FF
#define RED  		0xF800
#define MAGENTA		0xF81F
#define GREEN		0x07E0
#define CYAN 		0x7FFF
#define YELLOW		0xFFE0
#define BROWN		0XBC40
#define BRRED		0XFC07
#define GRAY 	    0X8430
#define DARKBLUE	0X01CF
#define LIGHTBLUE	0X7D7C
#define GRAYBLUE    0X5458
#define LIGHTGREEN  0X841F
#define LGRAY 		0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

/**
 * 
 * Display namespace includes all necessary functions to interface with the display
 * 
 */
namespace Display {
    /**
     * PWM slice to map brightness to [0, 100] range
     */
    static uint slice_num;

    /**
     * Send command to display via SPI
     */
    static void sendCommand(uint8_t reg);

    /**
     * Send (length) bytes to display via SPI
     */
    static void sendData(uint8_t* data, uint32_t length);
    
    /**
     * Send one byte to display via SPI (convenience wrapper around sendData)
     */
    static void sendData8Bit(uint8_t data);

    /**
     * Send two bytes to display via SPI. Reverses the byte order.
     */
    static void sendData16Bit(uint16_t data);

    /**
     * Initializes display registers
     */
    static void initReg();

    /**
     * Sends desired scan direction to display
     */
    static void setAttributes(uint8_t scanDirection);

    /**
     * Selects window to be drawn on display by providing a range [start, end] for each dimension
     */
    static void setWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

    /**
     * Initialization routine for display
     */
    void init();
    /**
     * Performs a hardware reset through reset pin
     */
    void reset();

    /**
     * Sets the brightness of the display
     * Note: The display goes dark at brightness = 0
     * and has full brightness at brightness = 100.
     * If brightness > 100, it is clipped to 100.
     */
    void setBrightness(uint8_t brightness);

    /**
     * Clears the display with the given color.
     */
    void clear(uint16_t color);

    /**
     * Updates a single pixel at position (x, y) with the given color
     */
    void setPixel(uint16_t x, uint16_t y, uint16_t color);
}

/**
 * 
 * The above code contains source-code from the WaveShare team demo code, 
 * whose license is included at the bottom of this file.
 * 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of theex Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
 * 
 */

#endif