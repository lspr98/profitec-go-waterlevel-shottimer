#ifndef _CONFIG_H
#define _CONFIG_H

// *******************************
//    Waterlevel sensor settings
// *******************************
// Profitec Go water reservoir is ~25cm deep
// The intake for the pump sticks out about 2cm
// The sensor sticks around ~2cm into the tank
// So at 25cm - 2cm - 2cm = 21cm = 210mm, the pump starts sucking air (0% fill level)
#define LEVEL_MAX_DIST_MM 210
// DF-A02YYUW sensor has a blind distance of 3cm = 30mm
#define LEVEL_MIN_DIST_MM 30
// Number of times to try reading a value from the sensor
#define LEVEL_N_RETRY 10
// Delay between two consecutive read attempts
#define LEVEL_READ_DELAY_MS 10
// Percentage value below which the ring changes color from OK to BAD
#define LEVEL_BAD_PERC 25

// *******************************
//       IMU sensor settings
// *******************************
// Number of samples to read from IMU
#define IMU_N_SAMPLES 10
// Delay between two consecutive reads
#define IMU_READ_DELAY_MS 10
// Minimum required standard deviation among samples to detect a vibration
#define WAKE_THRESH 0.5

// *******************************
//         Timer settings
// *******************************
// If you don't need the timer, you can disable it.
#define ENABLE_TIMER true
// How much milliseconds one increment of the counter needs
// For a precise shot time measurement, 1000ms should be used for a full second
// My Profitec GO seems to count a bit faster than 1000ms
#define TIMER_INCREMENT_MS 975
// The timer is started when the system is awake and a vibration is detected
// The trigger delay defines how many milliseconds the system has to vibrate
// while being awake until the timer is activated. This prevents the timer
// from starting when e.g. the portafilter is removed/inserted
#define TIMER_TRIGGER_DELAY_MS 2000


// *******************************
//         Counter settings
// *******************************
// Center point position of counter in pixels
#define COUNTER_CENTER_X 120
#define COUNTER_CENTER_Y 120
// Color of counter
#define COUNTER_COLOR WHITE

// *******************************
//     RingIndicator settings
// *******************************
// Center point position of ring-indicator in pixels
#define RING_CENTER_X 120
#define RING_CENTER_Y 120
// Radius of ring-indicator in pixels
#define RING_RADIUS 100
// Width of ring-indicator in pixels
#define RING_WIDTH 15
// Color of ring-indicator if fill level percentage > LEVEL_BAD_PERC
#define RING_COLOR_OK CYAN
// Color of ring-indicator if fill level percentage <= LEVEL_BAD_PERC
#define RING_COLOR_BAD BRRED

// *******************************
//    StatusIndicator settings
// *******************************
// Center point position of status-indicator in pixels
#define STATUS_CENTER_X 120
#define STATUS_CENTER_Y 215
// Color of status-indicator
#define STATUS_COLOR WHITE

// *******************************
//       Animation settings
// *******************************
// Target frame time for ring animation in milliseconds
#define FRAME_TIME_MS 20
// Animation time for ring-indicator when system wakes up
#define TRANSITION_TIME_LONG_MS 3000
// Animation time for ring-indicator after timer ends
#define TRANSITION_TIME_SHORT_MS 1500

// *******************************
//        Display settings
// *******************************
// Rotation of display
// Depending on the side at which the sensor is mounted, a different rotation
// value is required:
// Right:       90
// Left:        270
// Top:         0
// Bottom:      180
#define ROTATION 270
// Time after which the display goes dark when no vibration was detected
#define SLEEP_TIMEOUT_MS 60000
// Delay between dimming steps. Dimming is performed from 100 to 0 in decrements of 1.
#define DIM_STEP_DELAY_MS 1



#endif