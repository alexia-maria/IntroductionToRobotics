const byte latchPin = 11;
const byte clockPin = 10;
const byte dataPin = 12;
const byte segD1 = 4;
const byte segD2 = 5;
const byte segD3 = 6;
const byte segD4 = 7;
const int buttonPin1 = 2;
const int buttonPin2 = 3;
byte buttonState1 = HIGH;  // Modificat aici pentru a începe în starea HIGH (neapasat)
byte lastButtonState1 = HIGH;
unsigned long lastDebounceTime1 = 0;
byte buttonState2 = HIGH;  // Modificat aici pentru a începe în starea HIGH (neapasat)
byte lastButtonState2 = HIGH;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;
unsigned int currentNumber = 0;  // Modificat aici pentru a începe de la 0
byte displayDigits[] = { segD1, segD2, segD3, segD4 };
const byte displayCount = 4;
byte byteEncodings[] = { B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110 };
const bool displayOn = LOW;
const bool displayOff = HIGH;

const byte countingDelay = 100;
unsigned long lastIncrement = 0;

enum CounterState {
  RUNNING,
  PAUSED
};

CounterState counterState = PAUSED;  // Modificat aici pentru a începe în starea PAUSED

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], displayOff);
  }
  Serial.begin(9600);
}

void loop() {
  readButton1();
  int reading2 = digitalRead(buttonPin2);
  Serial.println(reading2);
  if (counterState == RUNNING && millis() - lastIncrement > countingDelay) {
    lastIncrement = millis();
    writeNumber(currentNumber);
    currentNumber++;
    if (currentNumber == 599) {
      currentNumber = 1000;
    }
    if (currentNumber == 10000) {
      currentNumber = 0;
    }
  } else {
    // Counter este în pauză, afișăm toate cifrele
    writeNumber(currentNumber);
  }

  //Serial.println(reading2);
  if (reading2 != lastButtonState2) {
    ///Serial.println("BUTON 2");
    // Serial.print("R2: ");
    // Serial.print(reading2);

    // Serial.println("L2: ");
    // Serial.print(lastButtonState2);
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 == 0) {
      Serial.println("AICIIIIIIIIIIIII");
      currentNumber = 0;
    }
  }
  lastButtonState2 = reading2;
}

void readButton1() {
  int reading1 = digitalRead(buttonPin1);

  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;

      if (buttonState1 == LOW) {
        if (counterState == RUNNING) {
          // Counter este în starea de funcționare, așa că îl oprim
          counterState = PAUSED;
          Serial.println("Counter paused");
        } else {
          // Counter este în pauză, îl repornim
          counterState = RUNNING;
          Serial.println("Counter resumed");
        }
      }
    }
  }

  lastButtonState1 = reading1;
}



void writeReg(byte encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

void tunOffDisplays() {
  for (byte i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], displayOff);
  }
}

void tunOnDisplay(byte displayNumber) {
  digitalWrite(displayDigits[displayNumber], displayOn);
}

void writeNumber(unsigned int number) {
  byte currentDisplay = 4;
  bool forceDisplay = number == 0;

  while (currentDisplay > 0 || forceDisplay) {
    currentDisplay--;
    byte lastDigit = number % 10;
    byte encoding = byteEncodings[lastDigit];

    if (currentDisplay == 2) {
      encoding |= B00000001;
    }

    tunOffDisplays();
    writeReg(encoding);
    tunOnDisplay(currentDisplay);

    number /= 10;
    forceDisplay = false;
    // delay(15);
    writeReg(B00000000);
  }
}
