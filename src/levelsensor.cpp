#include <levelsensor.h>
#include <serialutils.h>
#include <config.h>

namespace LevelSensor {
    
    void flush() {
        // Empty buffer by reading from it until it is empty.
        while(uart_is_readable(uart1)) {
            uart_getc(uart1);
        }
    }

    void init() {
        // Route HW UART1 to given RX,TX pins.
        gpio_set_function(PIN_LEVELSENSOR_TX, GPIO_FUNC_UART);
        gpio_set_function(PIN_LEVELSENSOR_RX, GPIO_FUNC_UART);
    }

    float getFillPercentage() {
        // Read raw distance
        float distance = getDistance();
        
        // Return -1 if there was an error reading
        if(distance < 0) return distance;

        // Convert to fill level to percentage. Note that the value has to be reversed since
        // LEVEL_MIN_DIST_MM indicates a full tank (100%) and LEVEL_MAX_DIST_MM an empty tank (0%)
        float perc = (1 - (distance - LEVEL_MIN_DIST_MM)/(LEVEL_MAX_DIST_MM - LEVEL_MIN_DIST_MM)) * 100;

        return perc;
    }

    float getDistance() {
        // Initialize uart
        uart_init(uart1, 9600);
        sleep_ms(100);

        // Clear old data from serial buffer
        flush();

        // Try to read a valid value LEVEL_N_RETRY times
        for(uint16_t i=0; i<LEVEL_N_RETRY; i++) {
            
            // Small timeout to wait for new data from sensor
            sleep_ms(LEVEL_READ_DELAY_MS);

            // Recieved no new data. Retry by continuing with next loop
            if(!uart_is_readable(uart1)) {
                continue;
            }
            
            // New data was recieved. According to the sensor wiki, there should be 4 bytes.
            // https://wiki.dfrobot.com/_A02YYUW_Waterproof_Ultrasonic_Sensor_SKU_SEN0311
            uart_read_blocking(uart1, serialData, 4);

            // Check if header byte is correct
            if(serialData[0]==0xff) {
                // Calculate checksum
                int checksum = (serialData[0]+serialData[1]+serialData[2])&0x00FF;
                // Check if checksum matches
                if(checksum == serialData[3]) {
                    // Clip distance to [minDistanceMM, maxDistanceMM] range
                    float distance = (serialData[1]<<8)+serialData[2];
                    if(distance > LEVEL_MAX_DIST_MM) {
                        distance = LEVEL_MAX_DIST_MM;
                    } else if(distance < LEVEL_MIN_DIST_MM){
                        distance = LEVEL_MIN_DIST_MM;
                    }
                    // Close uart
                    uart_deinit(uart1);
                    return distance;
                }
            }
        }

        // Close uart
        uart_deinit(uart1);
        // failed to read a valid value.
        sout.err() <= "Failed to read from level sensor";
        return -1;
    }
}