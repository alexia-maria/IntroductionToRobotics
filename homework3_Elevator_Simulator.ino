const int buttonPin1 = 5;
const int buttonPin2 = 6;
const int buttonPin3 = 7;
const int ledPin1 = 13;
const int ledPin2 = 11;
const int ledPin3 = 9;
const int ledPin4 = 8;

byte buttonState1 = LOW;
byte buttonState2 = LOW;
byte buttonState3 = LOW;
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;
byte ledState1 = HIGH;
byte ledState2 = LOW;
byte ledState3 = LOW;
byte ledState4 = HIGH;
int counter = 1;
byte reading1 = LOW;
byte reading2 = LOW;
byte reading3 = LOW;
byte sum = LOW;
byte lastReading = LOW;
byte lastReading1 = LOW;
byte lastReading2 = LOW;
byte lastReading3 = LOW;
int pozitieDefault = 0;
int numFloors = 3;
int flickerNum = 3;
int rest;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 100;
volatile bool buttonPressed = false;



void palpaieLED(int ledPin, int numPalpitatii) {
  for (int i = 0; i < numPalpitatii; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}



void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), handleInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), handleInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin3), handleInterrupt, FALLING);
}

void loop() {
  reading1 = digitalRead(buttonPin1);
  reading2 = digitalRead(buttonPin2);
  reading3 = digitalRead(buttonPin3);
  sum = reading1 + reading2 + reading3;

  if (sum != lastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();

    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH && ledState1 != HIGH) {
        if (counter != 1)
          if (ledState2 == HIGH) {
            ledState2 = !ledState2;
          }

        if (ledState3 == HIGH) {
          ledState3 = !ledState3;
        }


        while (counter % numFloors != 1) {
          counter++;

          palpaieLED(ledPin4, flickerNum);
        }
        ledState1 = HIGH;
        Serial.println(counter);
        Serial.println("Etajul ");
        Serial.println(counter % 4);
        digitalWrite(ledPin1, HIGH);
      }
    }
  }

  if (reading2 != buttonState2) {
    buttonState2 = reading2;
    if (buttonState2 == HIGH && ledState2 != HIGH) {
      if (ledState1 == HIGH) {
        ledState1 = !ledState1;
      }

      if (ledState3 == HIGH) {
        ledState3 = !ledState3;
      }
      while (counter % numFloors != 2) {
        counter++;

        palpaieLED(ledPin4, flickerNum);
      }
      ledState2 = HIGH;
      digitalWrite(ledPin2, HIGH);
    }
  }

  if (reading3 != buttonState3) {
    buttonState3 = reading3;
    if (buttonState3 == HIGH && ledState3 != HIGH) {
      if (ledState1 == HIGH) {
        ledState1 = !ledState1;
      }
      if (ledState2 == HIGH) {
        ledState2 = !ledState2;
      }
      while (counter % numFloors != 0) {
        counter++;

        palpaieLED(ledPin4, flickerNum);
      }
      ledState3 = HIGH;
      digitalWrite(ledPin3, HIGH);
    }
  }
  buttonPressed = false;
  digitalWrite(ledPin1, ledState1);
  digitalWrite(ledPin2, ledState2);
  digitalWrite(ledPin3, ledState3);
  digitalWrite(ledPin4, ledState4);
  lastReading = sum;
  lastReading1 = reading1;
  lastReading2 = reading2;
  lastReading3 = reading3;
}


void handleInterrupt() {
  buttonPressed = true;
}
