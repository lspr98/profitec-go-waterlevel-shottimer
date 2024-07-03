#ifndef _LEVELSENSOR_H
#define _LEVELSENSOR_H

#define PIN_LEVELSENSOR_TX 4u
#define PIN_LEVELSENSOR_RX 5u

#include <Arduino.h>

/**
 * 
 * LevelSensor namespace includes all necessary functions to read the water level.
 * 
 */
namespace LevelSensor {
    // 
    static unsigned char serialData[4];
    
    /**
     * Clears RX buffer by reading until empty
     */
    static void flush();

    /**
     * Initializes UART interface for level sensor
     */
    void init();

    /**
     * Tries to read the distance in millimeters from the sensor.
     * Returns:
     *      -1                                          if no value could be read
     *      LEVEL_MAX_DIST_MM                           if distance > LEVEL_MAX_DIST_MM
     *      LEVEL_MIN_DIST_MM                           if distance < LEVEL_MIN_DIST_MM
     *      (LEVEL_MIN_DIST_MM, LEVEL_MAX_DIST_MM)      otherwise
     */
    float getDistance();

    /**
     * Tries to read the distance in millimeters from the sensor and calculates a percentage
     * value based on LEVEL_MAX_DIST_MM and LEVEL_MIN_DIST_MM.
     * Returns:
     *      -1                                          if no value could be read
     *      0                                           if distance > LEVEL_MAX_DIST_MM
     *      100                                         if distance < LEVEL_MIN_DIST_MM
     *      (0, 100)                                    otherwise
     */
    float getFillPercentage();

}

#endif