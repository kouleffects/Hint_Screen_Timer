/**
 * "Toggle Switch State" puzzle 
 * 
 * This puzzle requires players to set a series of toggle switches to the correct on/off state. 
 * The solution can be set by pressing a "save" button, which stores the current state
 * of the switches to EEPROM, restored every time the puzzle is started.
 */

#include <EEPROM.h>

// CONSTANTS
const byte numSwitches = 7;  // Increase the number of switches
const byte switchPins[numSwitches] = {2, 3, 4, 5, 6, 7, 8};  // Add two more switch pins
const byte savePin = A0;
const byte relayPin = A1;
const byte eepromSignature = 121;

// GLOBALS
bool lastSwitchState[numSwitches];
enum State { Initialising, Active, Solved };
State state = Initialising;
bool solution[numSwitches] = {true, true, true, true, true, true, true};  // Adjust the solution array

void savePattern() {
  EEPROM.write(0, 0);
  for (int i = 0; i < numSwitches; i++) {
    solution[i] = lastSwitchState[i];
    EEPROM.write(i + 1, lastSwitchState[i]);
  }
  EEPROM.write(0, eepromSignature);
  Serial.println(F("Pattern saved to EEPROM"));
}

void loadPattern() {
  if (EEPROM.read(0) == eepromSignature) {
    for (int i = 0; i < numSwitches; i++) {
      solution[i] = EEPROM.read(i + 1);
    }
    Serial.println(F("Solution loaded from EEPROM"));
  } else {
    Serial.println(F("EEPROM data not valid"));
  }
}

void readState() {
  for (int i = 0; i < numSwitches; i++) {
    int switchValue = digitalRead(switchPins[i]);
    if (switchValue != lastSwitchState[i]) {
      Serial.print("Switch changed to ");
      Serial.println(switchValue);
      lastSwitchState[i] = (bool)switchValue;
    }
  }
}

bool isPuzzleSolved() {
  bool solved = true;
  for (int i = 0; i < numSwitches; i++) {
    if (lastSwitchState[i] != solution[i]) {
      solved = false;
    }
  }
  return solved;
}

void setup() {
  for (int i = 0; i < numSwitches; i++) {
    pinMode(switchPins[i], INPUT);
  }
  pinMode(savePin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Starting connection");

  loadPattern();

  state = Active;
}

void loop() {
  readState();

  if (digitalRead(savePin) == LOW) {
    while (digitalRead(savePin) == LOW) { delay(100); }
    savePattern();
  }

  bool puzzleIsSolved = isPuzzleSolved();

  if (state == Active && puzzleIsSolved) {
    Serial.println(F("Puzzle has been solved!"));
    digitalWrite(relayPin, HIGH);
    state = Solved;
  } else if (state == Solved && !puzzleIsSolved) {
    Serial.println(F("Puzzle has been unsolved!"));
    digitalWrite(relayPin, LOW);
    state = Active;
  }
}
