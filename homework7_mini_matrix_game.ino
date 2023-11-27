#include "LedControl.h"

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  
int playerPosition;
int matrix[8][8];
const int minWallPoints = 63;
int i, j, counter;
byte matrixBrightness = 8;
int playerPositionLine;
int playerPositionColumn;
bool ledState = false;  
bool bombLedState = false;
int ledFrequency = 1;
int bombFrequency = 10;
bool buttonPressed = false;
bool blinked = false;
int blinkCount = 0;
int totalBlinkCount = 5; 
int gameOverCounter = 0;
unsigned long lastMillis = 0;  // Last blink moment
unsigned long lastDebounceTime = 0;
unsigned long lastDebounceTimeDirection = 0;
const unsigned long debounceDelaySW = 200;

const int pinX = A0;  
const int pinY = A1;  
const int pinSW = 6;

void setup() {
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP);
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      matrix[i][j] = 0; 
    }
  }
  playerPositionLine = 0;
  playerPositionColumn = 0;
  gameOverCounter = 0;
  counter = 0;

  lc.shutdown(0, false);  
  lc.setIntensity(0, 8);  
  lc.clearDisplay(0);     

  randomSeed(analogRead(0));  // Initialize the random generator

  resetPositions();   //Function to reset the game once you press the reset button
}

void loop() {
  unsigned long currentMillis = millis();
  blink_led(playerPositionLine, playerPositionColumn, ledFrequency);

  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);
  int reading = digitalRead(pinSW);

  // Serial.print("X: ");
  // Serial.print(xValue);
  // Serial.print("\tY: ");
  // Serial.println(yValue);

  // Serial.println(reading);

  // I determine the direction based on the read values
  if (xValue < 300 && (currentMillis - lastDebounceTimeDirection) > debounceDelaySW) {
    Serial.println("Left");
    if (matrix[playerPositionLine][playerPositionColumn - 1] == 1 || playerPositionColumn == 0) {
      Serial.println("Left is a wall");
    } else if (matrix[playerPositionLine][playerPositionColumn - 1] != 1 || playerPositionColumn != 0) {
      lc.setLed(0, playerPositionLine, playerPositionColumn, false);  //if its not a wall. then the player can move 
      matrix[playerPositionLine][playerPositionColumn] = 0; //the position of the player becomes 0
      Serial.println("Set to 0 value");
      Serial.print(playerPositionLine);
      Serial.println(playerPositionColumn);
      Serial.println(matrix[playerPositionLine][playerPositionColumn]);
      playerPositionColumn = playerPositionColumn - 1;
      lc.setLed(0, playerPositionLine, playerPositionColumn, true);  //the new position of the player
    }
    if (reading == 0 && (currentMillis - lastDebounceTime) > debounceDelaySW) {
      Serial.println("Left Pressed");
      for (int l = playerPositionColumn; l >= 0; l--) {
        if (matrix[playerPositionLine][l] != 0) { //if we find a wall
          blink_led_bomb(playerPositionLine, l, bombFrequency); //the bomb destroys each wall
          matrix[playerPositionLine][l] = 0;
          lc.setLed(0, playerPositionLine, l, false); //the wall is set to 0 because it was destroyed
          Serial.println("Set to 0 at position");
          Serial.println(playerPositionLine);
          Serial.println(l);
          Serial.println(matrix[playerPositionLine][l]);
        }
      }
      buttonPressed = !buttonPressed;
      lastDebounceTime = millis();
    }
    lastDebounceTimeDirection = millis();
  } else if (xValue > 700 && (currentMillis - lastDebounceTimeDirection) > debounceDelaySW) { //we do the same thing for the right. up and down direction(player movement and destruction of walls with bombs)
    Serial.println("Right");
    if (matrix[playerPositionLine][playerPositionColumn + 1] == 1 || playerPositionColumn == 7) {
      Serial.println("Right is a wall");
      Serial.println(playerPositionColumn + 1);

    } else {
      lc.setLed(0, playerPositionLine, playerPositionColumn, false);
      matrix[playerPositionLine][playerPositionColumn] = 0;
      Serial.println("Set to 0 value");
      Serial.print(playerPositionLine);
      Serial.println(playerPositionColumn);
      Serial.println(matrix[playerPositionLine][playerPositionColumn]);
      playerPositionColumn = playerPositionColumn + 1;
      lc.setLed(0, playerPositionLine, playerPositionColumn, true);
    }
    if (reading == 0 && !buttonPressed && (currentMillis - lastDebounceTime) > debounceDelaySW) {
      Serial.println("Right Pressed");
      for (int l = playerPositionColumn; l <= 7; l++) {
        if (matrix[playerPositionLine][l] != 0) {
          blink_led_bomb(playerPositionLine, l, bombFrequency);
          matrix[playerPositionLine][l] = 0;
          lc.setLed(0, playerPositionLine, l, false);
          Serial.println("Set to 0 at position");
          Serial.println(playerPositionLine);
          Serial.println(l);
          Serial.println(matrix[playerPositionLine][l]);
        }
      }
      buttonPressed = !buttonPressed;
      lastDebounceTime = millis();
    }
    lastDebounceTimeDirection = millis();
  }

  if (yValue < 300 && (currentMillis - lastDebounceTimeDirection) > debounceDelaySW) {
    Serial.println("Up");
    if (matrix[playerPositionLine - 1][playerPositionColumn] == 1 || playerPositionLine == 0) {
      Serial.println("Up is a wall");
      Serial.println(playerPositionLine - 1);

    } else {
      lc.setLed(0, playerPositionLine, playerPositionColumn, false);
      matrix[playerPositionLine][playerPositionColumn] = 0;
      Serial.println("Set to 0 value");
      Serial.print(playerPositionLine);
      Serial.println(playerPositionColumn);
      Serial.println(matrix[playerPositionLine][playerPositionColumn]);
      playerPositionLine = playerPositionLine - 1;
      lc.setLed(0, playerPositionLine, playerPositionColumn, true);
    }
    if (reading == 0 && !buttonPressed && (currentMillis - lastDebounceTime) > debounceDelaySW) {
      Serial.println("Up Pressed");
      for (int l = playerPositionLine; l >= 0; l--) {
        if (matrix[l][playerPositionColumn] != 0) {
          blink_led_bomb(l, playerPositionColumn, bombFrequency);
          matrix[l][playerPositionColumn] = 0;
          lc.setLed(0, l, playerPositionColumn, false);
          Serial.println("Set to 0 at position");
          Serial.println(playerPositionLine);
          Serial.println(l);
          Serial.println(matrix[l][playerPositionColumn]);
        }
      }
      buttonPressed = !buttonPressed;
      lastDebounceTime = millis();
    }
    lastDebounceTimeDirection = millis();
  } else if (yValue > 700 && (currentMillis - lastDebounceTimeDirection) > debounceDelaySW) {
    Serial.println("Down");
    if (matrix[playerPositionLine + 1][playerPositionColumn] == 1 || playerPositionLine == 7) {
      Serial.println("Down is a wall");
      Serial.println(playerPositionLine + 1);
    } else {
      lc.setLed(0, playerPositionLine, playerPositionColumn, false);
      matrix[playerPositionLine][playerPositionColumn] = 0;
      Serial.println("Set to 0 value");
      Serial.print(playerPositionLine);
      Serial.println(playerPositionColumn);
      Serial.println(matrix[playerPositionLine][playerPositionColumn]);
      playerPositionLine = playerPositionLine + 1;
      lc.setLed(0, playerPositionLine, playerPositionColumn, true);
    }
    if (reading == 0 && !buttonPressed && (currentMillis - lastDebounceTime) > debounceDelaySW) {
      Serial.println("Down Pressed");
      for (int l = playerPositionLine; l <= 7; l++) {
        if (matrix[l][playerPositionColumn] != 0) {
          blink_led_bomb(l, playerPositionColumn, bombFrequency);
          matrix[l][playerPositionColumn] = 0;
          lc.setLed(0, l, playerPositionColumn, false);
          Serial.println("Set to 0 at position");
          Serial.println(playerPositionLine);
          Serial.println(l);
          Serial.println(matrix[l][playerPositionColumn]);
        }
      }
      buttonPressed = !buttonPressed;
      lastDebounceTime = millis();
    }
    lastDebounceTimeDirection = millis();
  }

  buttonPressed = !buttonPressed;


  if(check_empty_Matrix() == 0 && gameOverCounter == 0){
    gameOverCounter++; //increment the counter so that the program displays "GAME OVER" only once
    Serial.println("GAME OVER");
  }
}

void resetPositions() {
  Serial.print("Reset Function");
  Serial.println("Counter: ");
  Serial.print(counter);
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (matrix[i][j] == 0) {
        lc.setLed(0, i, j, false); 
      }
    }
  }

  playerPositionLine = random(1, 7); //we randomly generate the line of the player
  playerPositionColumn = random(1, 7);  //we randomly generate the column of the player
  counter = 0;

  matrix[playerPositionLine - 1][playerPositionColumn] = 2; //set the position near the player to 2 because we want him to be able to move and if we let these position without any value, we risk randomly generating a 0 in them
  matrix[playerPositionLine][playerPositionColumn + 1] = 2;
  matrix[playerPositionLine][playerPositionColumn - 1] = 2;
  matrix[playerPositionLine + 1][playerPositionColumn] = 2;

  // we generate a random number between 32 and 45 to decide how many elements will be set to 1
  int numOnes = random(32, 45);

  // we randomly set the positions of the elements to 1
  for (int count = 0; count < numOnes; ++count) {
    int randomRow = random(0, 8);
    int randomCol = random(0, 8);

    // check if this position has not already been set to 1
    if (matrix[randomRow][randomCol] == 0) {
      matrix[randomRow][randomCol] = 1;
    } else {
      // if the position is already set to 1, we choose another position
      --count;
    }
  }

  lc.setLed(0, playerPositionLine, playerPositionColumn, true);

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (matrix[i][j] == 1) {
        lc.setLed(0, i, j, true);
        counter++;
        Serial.print(i);
        Serial.print(" ");
        Serial.print(j);
      }
    }
  }
  Serial.println("Counter: ");
  Serial.println(counter);
}

void blink_led(int row, int col, int frequency) { //function for the player led, he will blick during the whole program
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= 1000 / frequency / 2) {
    lastMillis = currentMillis;

    ledState = !ledState;              
    lc.setLed(0, row, col, ledState);  
  }
}

void blink_led_bomb(int row, int col, int frequency) { //function for the bombs, they will blick for a short period of time, then they will stop
  unsigned long currentMillis = millis();
  static int i = 0;
  while (i < 5) {
    if (currentMillis - lastMillis >= 1000 / frequency / 2) {
      lastMillis = currentMillis;
      bombLedState = !bombLedState;             
      lc.setLed(0, row, col, bombLedState); 
      i++;

      // if (i == 5) {
      //   // Serial.println("I is");
      //   // Serial.println(i);
      // }
    }

    currentMillis = millis();  // Update currentMillis in each iteration
  }
  i = 0; 
}

int check_empty_Matrix() {
  for (i = 0; i <= 7; i++) {
    for (j = 0; j <= 7; j++) {
      if (matrix[i][j] == 1) {
        return 1;
      }
    }
  }
  return 0;
}

