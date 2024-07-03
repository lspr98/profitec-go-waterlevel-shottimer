#include <display.h>
#include <config.h>
#include <serialutils.h>
#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>

// Modified version of the Waveshare source code!

namespace Display {

    static void sendCommand(uint8_t reg) {
        gpio_put(LCD_DC_PIN, 0);
        spi_write_blocking(LCD_SPI_PORT, &reg, 1);
    }

    static void sendData(uint8_t* data, uint32_t length) {
        gpio_put(LCD_DC_PIN, 1);
        spi_write_blocking(LCD_SPI_PORT, data, length);
    }

    static void sendData8Bit(uint8_t data) {
        sendData(&data, 1);
    }

    static void sendData16Bit(uint16_t data) {
        // Swap the two bytes in data
        uint16_t dataReversed = (data << 8) | (data >> 8);
        sendData((uint8_t*) &dataReversed, 2);
    }

    void setWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
        // Set X range
        sendCommand(0x2A);
        sendData8Bit(0x00);
        sendData8Bit(xStart);
        sendData16Bit(xEnd);

        //Set Y range
        sendCommand(0x2B);
        sendData8Bit(0x00);
        sendData8Bit(yStart);
        sendData16Bit(yEnd);

        sendCommand(0X2C);
        gpio_put(LCD_DC_PIN, 1);;
    }

    static void initReg() {
        sendCommand(0xEF);
        sendCommand(0xEB);
        sendData8Bit(0x14); 
        
        sendCommand(0xFE);			 
        sendCommand(0xEF); 

        sendCommand(0xEB);	
        sendData8Bit(0x14); 

        sendCommand(0x84);			
        sendData8Bit(0x40); 

        sendCommand(0x85);			
        sendData8Bit(0xFF); 

        sendCommand(0x86);			
        sendData8Bit(0xFF); 

        sendCommand(0x87);			
        sendData8Bit(0xFF);

        sendCommand(0x88);			
        sendData8Bit(0x0A);

        sendCommand(0x89);			
        sendData8Bit(0x21); 

        sendCommand(0x8A);			
        sendData8Bit(0x00); 

        sendCommand(0x8B);			
        sendData8Bit(0x80); 

        sendCommand(0x8C);			
        sendData8Bit(0x01); 

        sendCommand(0x8D);			
        sendData8Bit(0x01); 

        sendCommand(0x8E);			
        sendData8Bit(0xFF); 

        sendCommand(0x8F);			
        sendData8Bit(0xFF); 


        sendCommand(0xB6);
        sendData8Bit(0x00);
        sendData8Bit(0x20);

        sendCommand(0x36);
        sendData8Bit(0x08);

        sendCommand(0x3A);			
        sendData8Bit(0x05); 


        sendCommand(0x90);			
        sendData8Bit(0x08);
        sendData8Bit(0x08);
        sendData8Bit(0x08);
        sendData8Bit(0x08); 

        sendCommand(0xBD);			
        sendData8Bit(0x06);
        
        sendCommand(0xBC);			
        sendData8Bit(0x00);	

        sendCommand(0xFF);			
        sendData8Bit(0x60);
        sendData8Bit(0x01);
        sendData8Bit(0x04);

        sendCommand(0xC3);			
        sendData8Bit(0x13);
        sendCommand(0xC4);			
        sendData8Bit(0x13);

        sendCommand(0xC9);			
        sendData8Bit(0x22);

        sendCommand(0xBE);			
        sendData8Bit(0x11); 

        sendCommand(0xE1);			
        sendData8Bit(0x10);
        sendData8Bit(0x0E);

        sendCommand(0xDF);			
        sendData8Bit(0x21);
        sendData8Bit(0x0c);
        sendData8Bit(0x02);

        sendCommand(0xF0);   
        sendData8Bit(0x45);
        sendData8Bit(0x09);
        sendData8Bit(0x08);
        sendData8Bit(0x08);
        sendData8Bit(0x26);
        sendData8Bit(0x2A);

        sendCommand(0xF1);    
        sendData8Bit(0x43);
        sendData8Bit(0x70);
        sendData8Bit(0x72);
        sendData8Bit(0x36);
        sendData8Bit(0x37);  
        sendData8Bit(0x6F);


        sendCommand(0xF2);   
        sendData8Bit(0x45);
        sendData8Bit(0x09);
        sendData8Bit(0x08);
        sendData8Bit(0x08);
        sendData8Bit(0x26);
        sendData8Bit(0x2A);

        sendCommand(0xF3);   
        sendData8Bit(0x43);
        sendData8Bit(0x70);
        sendData8Bit(0x72);
        sendData8Bit(0x36);
        sendData8Bit(0x37); 
        sendData8Bit(0x6F);

        sendCommand(0xED);	
        sendData8Bit(0x1B); 
        sendData8Bit(0x0B); 

        sendCommand(0xAE);			
        sendData8Bit(0x77);
        
        sendCommand(0xCD);			
        sendData8Bit(0x63);		


        sendCommand(0x70);			
        sendData8Bit(0x07);
        sendData8Bit(0x07);
        sendData8Bit(0x04);
        sendData8Bit(0x0E); 
        sendData8Bit(0x0F); 
        sendData8Bit(0x09);
        sendData8Bit(0x07);
        sendData8Bit(0x08);
        sendData8Bit(0x03);

        sendCommand(0xE8);			
        sendData8Bit(0x34);

        sendCommand(0x62);			
        sendData8Bit(0x18);
        sendData8Bit(0x0D);
        sendData8Bit(0x71);
        sendData8Bit(0xED);
        sendData8Bit(0x70); 
        sendData8Bit(0x70);
        sendData8Bit(0x18);
        sendData8Bit(0x0F);
        sendData8Bit(0x71);
        sendData8Bit(0xEF);
        sendData8Bit(0x70); 
        sendData8Bit(0x70);

        sendCommand(0x63);			
        sendData8Bit(0x18);
        sendData8Bit(0x11);
        sendData8Bit(0x71);
        sendData8Bit(0xF1);
        sendData8Bit(0x70); 
        sendData8Bit(0x70);
        sendData8Bit(0x18);
        sendData8Bit(0x13);
        sendData8Bit(0x71);
        sendData8Bit(0xF3);
        sendData8Bit(0x70); 
        sendData8Bit(0x70);

        sendCommand(0x64);			
        sendData8Bit(0x28);
        sendData8Bit(0x29);
        sendData8Bit(0xF1);
        sendData8Bit(0x01);
        sendData8Bit(0xF1);
        sendData8Bit(0x00);
        sendData8Bit(0x07);

        sendCommand(0x66);			
        sendData8Bit(0x3C);
        sendData8Bit(0x00);
        sendData8Bit(0xCD);
        sendData8Bit(0x67);
        sendData8Bit(0x45);
        sendData8Bit(0x45);
        sendData8Bit(0x10);
        sendData8Bit(0x00);
        sendData8Bit(0x00);
        sendData8Bit(0x00);

        sendCommand(0x67);			
        sendData8Bit(0x00);
        sendData8Bit(0x3C);
        sendData8Bit(0x00);
        sendData8Bit(0x00);
        sendData8Bit(0x00);
        sendData8Bit(0x01);
        sendData8Bit(0x54);
        sendData8Bit(0x10);
        sendData8Bit(0x32);
        sendData8Bit(0x98);

        sendCommand(0x74);			
        sendData8Bit(0x10);	
        sendData8Bit(0x85);	
        sendData8Bit(0x80);
        sendData8Bit(0x00); 
        sendData8Bit(0x00); 
        sendData8Bit(0x4E);
        sendData8Bit(0x00);					
        
        sendCommand(0x98);			
        sendData8Bit(0x3e);
        sendData8Bit(0x07);

        sendCommand(0x35);	
        sendCommand(0x21);

        sendCommand(0x11);
        sleep_ms(120);
        sendCommand(0x29);
        sleep_ms(20);
    }

    static void setAttributes(uint8_t scanDirection) {

        sendCommand(0x36);
        if(scanDirection == HORIZONTAL) {
            sendData8Bit(0xc8);
        } else {
            sendData8Bit(0x68);
        }
    }

    void init() {
        // Configure system clock for SPI
        set_sys_clock_khz(PLL_SYS_KHZ, true);
        clock_configure(
            clk_peri,
            0,                                                
            CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 
            PLL_SYS_KHZ * 1000,                               
            PLL_SYS_KHZ * 1000                              
        );

        // Set SPI pins as output
        gpio_init(LCD_RST_PIN);
        gpio_set_dir(LCD_RST_PIN, 1);
        gpio_init(LCD_DC_PIN);
        gpio_set_dir(LCD_DC_PIN, 1);
        gpio_init(LCD_CS_PIN);
        gpio_set_dir(LCD_CS_PIN, 1);
        gpio_init(LCD_BL_PIN);
        gpio_set_dir(LCD_BL_PIN, 1);

        // Chip select + backlight
        gpio_put(LCD_CS_PIN, 1);
        gpio_put(LCD_DC_PIN, 0);
        gpio_put(LCD_BL_PIN, 1);

        // Initialize PWM pin and map it to range [0, 100]
        gpio_set_function(LCD_BL_PIN, GPIO_FUNC_PWM);
        slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);
        pwm_set_wrap(slice_num, 100);
        pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
        pwm_set_clkdiv(slice_num, 50);
        pwm_set_enabled(slice_num, true);

        // Initialize SPI port for display
        spi_init(LCD_SPI_PORT, 270000 * 1000);
        gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
        gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);

        // Perform hardware reset
        reset();

        // Set scan direction
        setAttributes(VERTICAL);

        // Initialize display registers
        initReg();

    }

    void reset() {
        // Perform hardware reset by HIGH-LOW-HIGH sequence
        gpio_put(LCD_RST_PIN, 1);
        sleep_ms(100);
        gpio_put(LCD_RST_PIN, 0);
        sleep_ms(100);
        gpio_put(LCD_RST_PIN, 1);
        gpio_put(LCD_CS_PIN, 0);
        sleep_ms(100);
    }

    void clear(uint16_t color) {
        // Create buffer with blank image over complete display size
        uint16_t j;
        uint16_t Image[LCD_WIDTH*LCD_HEIGHT_EXT];
        
        // Paint buffer with desired color
        color = ((color<<8)&0xff00)|(color>>8);
        for (j = 0; j < LCD_HEIGHT_EXT*LCD_WIDTH; j++) {
            Image[j] = color;
        }
        
        // Select complete display for painting
        setWindow(0, 0, LCD_WIDTH, LCD_HEIGHT_EXT);
        // Write complete black image row-wise
        gpio_put(LCD_DC_PIN, 1);;
        for(j = 0; j < LCD_HEIGHT_EXT; j++){
            sendData((uint8_t *)&Image[j*LCD_WIDTH], LCD_WIDTH*2);
        }
    }

    void setPixel(uint16_t x, uint16_t y, uint16_t color) {
        // Decide on pixel position based on rotation and select it.
        switch(ROTATION) {
            case 90:
                // Mounted to right side of machine
                setWindow(y,LCD_HEIGHT-x,y,LCD_HEIGHT-x);
                break;

            case 180:
                // Mounted to bottom of machine
                setWindow(
                    LCD_WIDTH-x,
                    LCD_HEIGHT-y,
                    LCD_WIDTH-x,
                    LCD_HEIGHT-y);
                break;

            case 270:
                // Mounted to left side of machine
                setWindow(LCD_WIDTH-y,x,LCD_WIDTH-y,x);
                break;

            default:
                // Mounted to top of machine
                // Includes 0° rotation
                setWindow(x,y,x,y);
                break;
        }
        // Paint pixel in desired color.
        sendData16Bit(color);
    };

    void setBrightness(uint8_t brightness) {
        // Clip brightness if necessary
        if(brightness > 100) {
            brightness = 100;
            sout.warn() <= "Display::setBrightness called with brightness > 100";
        }

        // Set PWM
        pwm_set_chan_level(slice_num, PWM_CHAN_B, brightness);
    };

}