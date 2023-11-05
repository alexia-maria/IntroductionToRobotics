
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
const int segSize = 8;
int index = 0;
bool commonAnode = false;
const int noOfDigits = 10;
byte state = HIGH;
byte dpState = LOW;
byte swState = LOW;
byte lastSwState = LOW;
int xValue = 0;
int yValue = 0;
byte currentPinPosition = 7;
boolean segmentState[8] = { false, false, false, false, false, false, false, false };
int savedSegment = 0;
bool isButtonPressed = false;
int lastPinPosition = 0;
bool joyMoved = 0;
int counterT = 0;
int digit = 0;
int minThreshold = 512;
int maxThreshold = 800;
const unsigned long debounceDelay = 1000;
const unsigned long debounceDelaySW = 500;
unsigned long lastDebounceTime = 0;


int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

String pinConnections[4][8] = {
  //   a   b    c     d     e     f    g     dp
  { "N/A", "A", "G", "G", "G", "A", "A", "N/A" },     // UP
  { "G", "G", "D", "N/A", "D", "G", "D", "N/A" },     // DOWN
  { "F", "F", "E", "E", "N/A", "N/A", "N/A", "C" },   // LEFT
  { "B", "N/A", "DP", "C", "C", "B", "N/A", "N/A" },  // RIGHT
};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  digitalWrite(pinDP, HIGH);
  pinMode(pinSW, INPUT_PULLUP);
  if (commonAnode == true) {
    state = !state;
  }
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  swState = digitalRead(pinSW);
  if (swState == lastSwState) {
    counterT++;
    if (swState == LOW && counterT % 2 == 1 && (millis() - lastDebounceTime) > debounceDelaySW) {
      Serial.println("Am apasat");
      Serial.println("Pozitia ");
      Serial.println(currentPinPosition);
      segmentState[currentPinPosition] = !segmentState[currentPinPosition];
      Serial.println(segmentState[currentPinPosition]);
    }
    lastSwState = swState;
    lastDebounceTime = millis();
    joyMoved = 1;
  }


  if (yValue > 580 && joyMoved == 0 && (millis() - lastDebounceTime) > debounceDelay) {
    Serial.println(currentPinPosition);
    currentPinPosition = displayNumber(2, currentPinPosition);
    Serial.println("Pozitie noua: ");
    Serial.println(currentPinPosition);
    joyMoved = 1;
    lastDebounceTime = millis();
  } else if (yValue < 400 && joyMoved == 0 && (millis() - lastDebounceTime) > debounceDelay) {
    currentPinPosition = displayNumber(3, currentPinPosition);
    joyMoved = 1;
    lastDebounceTime = millis();
  }
  if (xValue < 400 && joyMoved == 0 && (millis() - lastDebounceTime) > debounceDelay) {
    Serial.println(currentPinPosition);
    currentPinPosition = displayNumber(0, currentPinPosition);
    joyMoved = 1;
    lastDebounceTime = millis();
  } else if (xValue > 580 && joyMoved == 0 && (millis() - lastDebounceTime) > debounceDelay) {
    currentPinPosition = displayNumber(1, currentPinPosition);
    joyMoved = 1;
    lastDebounceTime = millis();  
  }
  joyMoved = 0;
}

void palpaieLED(int pin, int interval) {
  static unsigned long previousMillis = 0;
  static int ledState = LOW;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = (ledState == LOW) ? HIGH : LOW;
    digitalWrite(pin, ledState);
  }
}

int displayNumber(byte segment, byte position) {
  int pozitieVector = -1;

  int pozitieCurenta = position;

  if (pozitieCurenta != pozitieVector) {
    for (int i = 0; i < 8; i++) {
      if (segmentState[i] == false) {
        digitalWrite(segments[i], LOW);
      } else {
        Serial.println("E aprinsa pozitia: ");
        Serial.println(i);
        digitalWrite(segments[i], HIGH);
      }
    }

    if (pinConnections[segment][position] != "N/A" && pinConnections[segment][position] != "DP") {
      char connectionChar = pinConnections[segment][position].charAt(0);
      int asciiCode = static_cast<int>(connectionChar);
      pozitieVector = asciiCode - 65;
      Serial.println("Pozitie vector: ");
      Serial.println(pozitieVector);
      digitalWrite(segments[pozitieVector], HIGH);
    } else if (pinConnections[segment][position] == "DP") {
      pozitieVector = 7;
      digitalWrite(segments[pozitieVector], HIGH);
    } else if (pinConnections[segment][position] == "N/A") {
      pozitieVector = position;
      digitalWrite(segments[pozitieVector], HIGH);
    }
  }
  return pozitieVector;
}
