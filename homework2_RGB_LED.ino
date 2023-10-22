const int redPin = 12;         
const int greenPin = 11;      
const int bluePin = 10;      

const int redPotPin = A0;     
const int greenPotPin = A1; 
const int bluePotPin = A2;    

const int minPotValue = 0;   
const int maxPotValue = 1023; 

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Inițializarea comunicării seriale
  Serial.begin(9600);
}

void loop() {
  int redValue = analogRead(redPotPin);
  int greenValue = analogRead(greenPotPin);
  int blueValue = analogRead(bluePotPin);

  int redIntensity = map(redValue, minPotValue, maxPotValue, 0, 255);
  int greenIntensity = map(greenValue, minPotValue, maxPotValue, 0, 255);
  int blueIntensity = map(blueValue, minPotValue, maxPotValue, 0, 255);

  analogWrite(redPin, redIntensity);
  analogWrite(greenPin, greenIntensity);
  analogWrite(bluePin, blueIntensity);

  float redVoltage = (redIntensity * 5.0)/1023.0;
  float greenVoltage = (greenIntensity * 5.0)/1023.0;
  float blueVoltage = (blueIntensity * 5.0)/1023.0;

  Serial.print("Red Value: ");
  Serial.print(blueValue);
  Serial.print("  Red Intensity: ");
  Serial.print(redIntensity);
  Serial.print("  Red Voltage: ");
  Serial.print(redVoltage);

  Serial.print("  Green Value: ");
  Serial.print(greenValue);
  Serial.print("  Green Intensity: ");
  Serial.print(greenIntensity);
  Serial.print("  Green Voltage: ");
  Serial.print(greenVoltage);

  Serial.print("  Blue Value: ");
  Serial.print(redValue);
  Serial.print("  Blue Intensity: ");
  Serial.println(blueIntensity);
  Serial.print("  Blue Voltage: ");
  Serial.println(blueVoltage);

  delay(100); 
}

