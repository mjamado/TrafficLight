// Arduino pin where the cars red LED is connected
const byte CARS_RED_PIN = 6;
// Arduino pin where the cars yellow LED is connected
const byte CARS_YELLOW_PIN = 5;
// Arduino pin where the cars green LED is connected
const byte CARS_GREEN_PIN = 4;
// Arduino pin where the pedestrians red LED is connected
const byte PEDS_RED_PIN = 3;
// Arduino pin where the pedestrians green LED is connected
const byte PEDS_GREEN_PIN = 2;

// Don't connect anything to this Arduino pin
const byte NO_LIGHT_PIN = 7;

// how long should the green light for cars stay on (seconds)
const int CARS_GREEN_TIME = 6;
// how long should the yellow light for cars blink (seconds)
const int CARS_BLINK_YELLOW_TIME = 2;
// how long should be the delay between the red light for cars and the green for pedestrians (seconds)
const int PEDS_HOLD_GREEN_TIME = 1;
// how long should the green light for pedestrians stay on (seconds)
const int PEDS_GREEN_TIME = 4;
// how long should the green light for pedestrians blink (seconds)
const int PEDS_BLINK_GREEN_TIME = 2;
// how long should be the delay between the red light for pedestrians and the green for cars (seconds)
const int CARS_HOLD_GREEN_TIME = 1;

// what is the on/off cycle of blinking lights (milliseconds)
const int BLINKING_TIMING = 500;

// what is the total time of the cycle (milliseconds)
const int TOTAL_TIME = (CARS_GREEN_TIME +
  CARS_BLINK_YELLOW_TIME +
  PEDS_HOLD_GREEN_TIME +
  PEDS_GREEN_TIME +
  PEDS_BLINK_GREEN_TIME +
  CARS_HOLD_GREEN_TIME) * 1000;

// store the last time the loop ran (milliseconds)
unsigned long lastTime = 0;
// store the point in the cycle we currently are (milliseconds)
int elapsedTime = 0;
// store the point in the cycle of the blinking lights (milliseconds)
int elapsedTimeBlinking = 0;

// Initial setup - this runs only once, on start up
void setup() {
  Serial.begin(9600);

  // mark the pins for output
  pinMode(CARS_RED_PIN, OUTPUT);
  pinMode(CARS_YELLOW_PIN, OUTPUT);
  pinMode(CARS_GREEN_PIN, OUTPUT);
  pinMode(PEDS_RED_PIN, OUTPUT);
  pinMode(PEDS_GREEN_PIN, OUTPUT);
  pinMode(NO_LIGHT_PIN, OUTPUT);
}

void calculateCyclesTimes() {
  unsigned long now = millis();
  // calculate the cycle time
  elapsedTime = (elapsedTime + (now - lastTime)) % TOTAL_TIME;
  // calculate the blinking cycle time
  elapsedTimeBlinking = elapsedTime % (BLINKING_TIMING * 2);
  // store the current time for the next loop
  lastTime = now;
}

// turn off all the lights
void everythingOff() {
  digitalWrite(CARS_RED_PIN, LOW);
  digitalWrite(CARS_YELLOW_PIN, LOW);
  digitalWrite(CARS_GREEN_PIN, LOW);
  digitalWrite(PEDS_RED_PIN, LOW);
  digitalWrite(PEDS_GREEN_PIN, LOW);
  digitalWrite(NO_LIGHT_PIN, LOW);
}

// which light should be on for cars
byte carsCurrentLight() {
  // if it should be green
  if (elapsedTime / 1000 < CARS_GREEN_TIME) {
    return CARS_GREEN_PIN;
  // if it should not be on, but it should be blinking yellow
  } else if (elapsedTime / 1000 < CARS_GREEN_TIME + CARS_BLINK_YELLOW_TIME) {
    // if it should be on the on part of the blinking cycle
    if (elapsedTimeBlinking < BLINKING_TIMING) {
      return CARS_YELLOW_PIN;
    } else {
      return NO_LIGHT_PIN;
    }
  } else {
    return CARS_RED_PIN;
  }
}

// which light should be on for pedestrians
byte pedsCurrentLight() {
  if (elapsedTime / 1000 < CARS_GREEN_TIME + CARS_BLINK_YELLOW_TIME + PEDS_HOLD_GREEN_TIME) {
    return PEDS_RED_PIN;
  } else if (elapsedTime / 1000 < CARS_GREEN_TIME + CARS_BLINK_YELLOW_TIME + PEDS_HOLD_GREEN_TIME + PEDS_GREEN_TIME) {
    return PEDS_GREEN_PIN;
  } else if (elapsedTime / 1000 < CARS_GREEN_TIME + CARS_BLINK_YELLOW_TIME + PEDS_HOLD_GREEN_TIME + PEDS_GREEN_TIME + PEDS_BLINK_GREEN_TIME) {
    if (elapsedTimeBlinking < BLINKING_TIMING) {
      return PEDS_GREEN_PIN;
    } else {
      return NO_LIGHT_PIN;
    }
  } else {
    return PEDS_RED_PIN;
  }
}

void loop() {
  // calculate in which time in the cycle we are
  calculateCyclesTimes();

  // turn off all the lights
  // (don't worry, we'll turn on some next)
  everythingOff();

  // turn on the correct car light
  digitalWrite(carsCurrentLight(), HIGH);
  // turn on the correct pedestrian light
  digitalWrite(pedsCurrentLight(), HIGH);
}
