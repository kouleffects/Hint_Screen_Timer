/**
 * "Toggle Switch State" puzzle 
 * 
 * This puzzle requires players to set a series of toggle switches to the correct on/off state. 
 * The solution can be set by pressing a "save" button, which stores the current state
 * of the switches to EEPROM, restored every time the puzzle is started.
 */

// Include the EEPROM library to enable us to save the pattern
#include <EEPROM.h>

// CONSTANTS
// The number of switches. Can be as many as you have digital pins available
const byte numSwitches = 5;
// The pins to which switches are connected
const byte switchPins[numSwitches] = { 2, 3, 4, 5, 6 };
// Pressing the button connected to this pin will save the current state to EEPROM
const byte savePin = A0;
// This pin will be driven LOW when the puzzle is solved
const byte relayPin = A1;
// Simple signature in first byte in EEPROM indicates stored data is valid
const byte eepromSignature = 121;

// GLOBALS
// An array to record the last known state of every switch
bool lastSwitchState[numSwitches];
// The current state of the puzzle
enum State { Initialising,
             Active,
             Solved };
State state = Initialising;
// The desired solution state - will be overwritten by value in EEPROM
bool solution[numSwitches] = { true, true, true, true, true };

// Set the puzzle solution to the current switch state and save to EEPROM
void savePattern() {
  // Erase the existing signature
  EEPROM.write(0, 0);
  // Store the current state of each switch to EEPROM
  for (int i = 0; i < numSwitches; i++) {
    solution[i] = lastSwitchState[i];
    EEPROM.write(i + 1, lastSwitchState[i]);
  }
  // Now write the signature back again to byte zero to show valid data
  EEPROM.write(0, eepromSignature);
  // Serial output
  Serial.println(F("Pattern saved to EEPROM"));
}

// Loads a saved switch pattern from EEPROM
void loadPattern() {
  // Check that the EEPROM starts with a valid signature
  if (EEPROM.read(0) == eepromSignature) {
    for (int i = 0; i < numSwitches; i++) {
      solution[i] = EEPROM.read(i + 1);
    }
    // Update the values of count and max delay
    Serial.println(F("Solution loaded from EEPROM"));
  } else {
    Serial.println(F("EEPROM data not valid"));
  }
}

void readState() {
  for (int i = 0; i < numSwitches; i++) {
    int switchValue = digitalRead(switchPins[i]);
    if (switchValue != lastSwitchState[i]) {
      Serial.print("Switch changed to");
      Serial.println(switchValue);
      lastSwitchState[i] = (bool)switchValue;
    }
  }
}

// Checks the last known state of switches against the solution
bool isPuzzleSolved() {
  // Assume the puzzle is solved
  bool solved = true;
  // Check each pin's state
  for (int i = 0; i < numSwitches; i++) {
    // If any switch fails to match its desired state
    if (lastSwitchState[i] != solution[i]) {
      //... then the puzzle has not been solved
      solved = false;
    }
  }
  return solved;
}

void setup() {
  // Initialise the input pins
  for (int i = 0; i < numSwitches; i++) {
    pinMode(switchPins[i], INPUT);
  }
  pinMode(savePin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  // Start the serial connection
  Serial.begin(9600);
  Serial.println("Starting connection");

  // Load the saved solution from EEPROM
  loadPattern();

  // Set the puzzle state
  state = Active;
}

void loop() {
  // Get the current state of the switches
  readState();

  // Is the save button pressed?
  if (digitalRead(savePin) == LOW) {
    // Simple debounce
    while (digitalRead(savePin) == LOW) { delay(100); }
    // Save the current pattern to EEPROM
    savePattern();
  }

  // Is the puzzle solved?
  bool puzzleIsSolved = isPuzzleSolved();

  // If it's just been solved
  if (state == Active && puzzleIsSolved) {
    Serial.println(F("Puzzle has been solved!"));
    digitalWrite(relayPin, HIGH);
    state = Solved;
  }
  // If it was solved, but no longer is
  else if (state == Solved && !puzzleIsSolved) {
    Serial.println(F("Puzzle has been unsolved!"));
    digitalWrite(relayPin, LOW);
    state = Active;
  }
}
