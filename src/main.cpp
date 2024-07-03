#include <Arduino.h>
#include <pico/rand.h>
#include <graphics.h>
#include <counter.h>
#include <serialutils.h>
#include <ringindicator.h>
#include <interpolator.h>
#include <Roboto_Light20pt7b.h>
#include <SoftwareSerial.h>
#include <QMI8658.h>
#include <display.h>
#include <levelsensor.h>
#include <statusindicator.h>
#include <config.h>

// UI elements
Counter counter;
Interpolator interpolator;
RingIndicator ring;
StatusIndicator status;

// Initial ring indicator color
uint16_t currRingColor = RING_COLOR_OK;
// Fill level percentage tracker
float currPerc, targetPerc = 0;
// Sleep states
bool isAwake, isVibrating, isVibratingPrev = false;
// Timing
long tLastVibration = 0;
long loopStart, loopEnd = 0;
// Vibration logging
float acc_xyz[IMU_N_SAMPLES][3], acc_mean_xyz[3], acc_std_xyz[3];


/**
 * Checks for vibrations by reading samples from the accelerometer, 
 * calculating the standard deviation of these samples and comparing it
 * to a threshold. 
 */
bool checkVibration() {
  // Read Sensor IMU_N_SAMPLES times
  for(int i=0; i<IMU_N_SAMPLES; i++) {
    // Get current XYZ accelerations
    QMI8658_read_acc_xyz(acc_xyz[i]);
    // Wait for a short time before reading next sample
    sleep_ms(IMU_READ_DELAY_MS);
  }

  // Calculate mean for each axis
  for(int i=0; i<3; i++) {
    acc_mean_xyz[i] = 0;
    for(int j=0; j<IMU_N_SAMPLES; j++) {
      acc_mean_xyz[i] += acc_xyz[j][i];
    }
    acc_mean_xyz[i] /= IMU_N_SAMPLES;
  }

  // Calculate std for each axis
  for(int i=0; i<3; i++) {
    acc_std_xyz[i] = 0;
    for(int j=0; j<IMU_N_SAMPLES; j++) {
      acc_std_xyz[i] += pow(acc_xyz[j][i] - acc_mean_xyz[i], 2);
    }
    acc_std_xyz[i] = sqrtf(acc_std_xyz[i] / IMU_N_SAMPLES);

    // Check if threshold is exceeded
    if(acc_std_xyz[i] > WAKE_THRESH) {
      return true;
    }
  }

  return false;
}


/**
 * Routine that shows and increases a timer each second if a continuous
 * vibration is detected. Finishes once the vibration stops for a second.
 */
void timerRoutine(long tStart) {
  // Time passed since timer was started
  long tPassed = millis() - tStart;
  // Approximate time needed to check for vibration
  long tVibeCheck = IMU_N_SAMPLES*IMU_READ_DELAY_MS;
  status.showStatus(StatusIndicatorState::TIMER, STATUS_COLOR);
  counter.update(((float)tPassed)/TIMER_INCREMENT_MS, COUNTER_COLOR);
  // Outer loop keeps the timer running, performing second-wise updates
  while(true) {
    bool isVibrating = false;
    // Inner loop keeps checking if the machine is still vibrating while a full second hasn't passed
    while(true) {
      // Perform vibration check
      isVibrating = checkVibration();
      // Update last vibration time if applicable
      if(isVibrating) tLastVibration = millis();
      // Calculate remaining time to next full second
      long tDiff = (millis() - tStart);
      tDiff = (floor((float)tDiff/TIMER_INCREMENT_MS)+1)*TIMER_INCREMENT_MS - tDiff;
      // Check if enough time for another vibecheck is left or a vibration was detected
      if(isVibrating || tVibeCheck >= tDiff) {
        // Wait remaining time to full second if applicable
        if(tDiff > 0) sleep_ms(tDiff);
        break;
      }
    }
    // Break timer loop if no vibration was detected in the past second
    if(!isVibrating) break;

    // Update timer
    tPassed = millis() - tStart;
    counter.update(((float)tPassed)/TIMER_INCREMENT_MS, COUNTER_COLOR);
  }
}

/**
 * Routine that tries to read a valid filllevel. If a valid value
 * is read, the filllevel is displayed after an animation.
 * If no valid value is read, an error screen is shown
 */
void levelRoutine(float transitionTime) {
  // Try to get fill level
  targetPerc = LevelSensor::getFillPercentage();
  
  // Choose ring color based on percentage
  if(targetPerc > LEVEL_BAD_PERC) {
    currRingColor = RING_COLOR_OK;
  } else {
    currRingColor = RING_COLOR_BAD;
  }

  if(targetPerc > -1) {
    status.showStatus(StatusIndicatorState::LEVEL, STATUS_COLOR);
    counter.update(currPerc, COUNTER_COLOR);
    ring.updatePrec(currPerc, currRingColor);
  } else {
    // Couldn't read filllevel. Show error state
    status.showStatus(StatusIndicatorState::ERROR, STATUS_COLOR);
    counter.hide();
    ring.updatePrec(100, currRingColor);
  }

  // Gradually wakeup display
  for(int i=0; i < 100; i++) {
    Display::setBrightness(i);
    sleep_ms(DIM_STEP_DELAY_MS);
  }

  if(targetPerc > -1) {
    // Transition
    interpolator.setFromTo(currPerc, targetPerc, transitionTime);
    while(!interpolator.next(currPerc)) {
        loopStart = millis();
        ring.updatePrec(currPerc, currRingColor);
        counter.update(currPerc, COUNTER_COLOR);
        loopEnd = millis();
        if(loopEnd-loopStart < FRAME_TIME_MS) {
            sleep_ms(FRAME_TIME_MS - (loopEnd-loopStart));
        }
    }
  }
}

/**
 * Wakes up the system by triggering the level routine and resetting the current fill percentage.
 */
void systemWake() {
  // Reset level
  currPerc = 0;
  isAwake = true;
  levelRoutine(TRANSITION_TIME_LONG_MS);
  
}


/**
 * Puts the system to sleep state by dimming the display and hiding UI elements
 * Note: The MCU is not actually put to sleep, just the display is turned off.
 */
void systemSleep() {
  // Gradually dim display
  for(int i=99; i > -1; i--) {
    Display::setBrightness(i);
    sleep_ms(DIM_STEP_DELAY_MS);
  }

  // Hide UI elements
  counter.hide();
  status.hide();
  ring.hide();

  // Wipe display
  Display::clear(BLACK);

  isAwake = false;
}


void setup() {

    Serial.begin(9600);

    // Initialize display
    Display::init();
    Display::clear(BLACK);

    // Initialize IMU
    QMI8658_init();

    // Initialize level sensor
    LevelSensor::init();

    // Update ring limits to make space for status symbol at the bottom
    ring.setLimits(0.2*PI, 1.8*PI);

    systemWake();

}

void loop() {

    isVibrating = checkVibration();
    if(isVibrating && !isVibratingPrev) tLastVibration = millis();

    if(!isAwake && isVibrating) {
      // System is sleeping and vibrating
      systemWake();
    } else if(isVibrating) {
      // System is awake and vibrating
      // Wait for timer trigger delay
      sleep_ms(TIMER_TRIGGER_DELAY_MS);
      // Start timer if enabled and still vibrating
      if(ENABLE_TIMER && checkVibration()) {
        timerRoutine(tLastVibration);
        // Vibration stopped, go back to show fill level
        levelRoutine(TRANSITION_TIME_SHORT_MS);
      }
      
    } else if(isAwake && ((millis() - tLastVibration) > SLEEP_TIMEOUT_MS)) {
      // Sleep due to timeout
      systemSleep();
    }

    isVibratingPrev = isVibrating;
}