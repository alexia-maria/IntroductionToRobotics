#include <EEPROM.h>

const int sensorPin = A0;           // Analog pin to which the light sensor is connected
const int ledPin = 11;              // Digital pin to which the LED is connected

int brightnessLimit = 0;            // Variable for the brightness limit
int samplingRate = 1;               // Variable for sampling rate
int address = 0;                     // EEPROM address where the brightness limit value is stored
int samplingAddress = 4;             // EEPROM address where the sampling rate value is stored
int ultrasonicLimit = 0;             // Variable for the ultrasonic sensor limit
int ultrasonicAddress = 10;           // EEPROM address where the ultrasonic sensor limit value is stored

const int maxReadings = 10;     //length of the array
int sensorReadings[maxReadings]; //array for the readings of the LDR Sensor
int currentReadingIndex = 0;

const int maxReadingsUltra = 10;   //length of the array
int sensorReadingsUltra[maxReadingsUltra]; //array for the readings of the Ultrasonic Sensor
int currentReadingIndexUltra = 0;

const int trigPin = 6;
const int echoPin = 7;

long duration = 0;
int distance = 0;

unsigned long startTime;
unsigned long interval; 

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  printMenu();
}

void loop() {
  // Continuously checks for incoming serial data
  if (Serial.available()) {
    // Reads an integer value from the serial buffer (user's menu choice)
    int choice = Serial.parseInt();
    // Calls function to print a message based on the user's choice
    printMessage(choice);
    // Re-displays the menu after executing the chosen action
    switch (choice) {
      case 1:
        printSubMenu1();
        while (true) {
          // Wait until the user inputs something
          if (Serial.available()) {
            Serial.flush();
            int subChoice = Serial.read() - '0';
            Serial.println("SubChoice read: " + String(subChoice));
            if (subChoice == 4) {
              Serial.println("Returning to the main menu");
              break;  // Exit the loop if the user chose to return to the main menu
            } else {
              Serial.println("Calling printSubMessage1");
              printSubMessage1(subChoice);
              Serial.println("Returned from printSubMessage1");
            }
          }
        }
        break;

      case 2:
        printSubMenu2();
        while (true) {
          if (Serial.available()) {
            int subChoice = Serial.read() - '0';
            Serial.println(subChoice);
            if (subChoice == 2) {
              Serial.println("Returning to the main screen");
              break;
            } else {
              printSubMessage2(subChoice);
            }
          }
        }
        break;

      case 3:
        printSubMenu3();
        while (true) {
          if (Serial.available()) {
            int subChoice = Serial.read() - '0';
            Serial.println(subChoice);
            if (subChoice == 4) {
              Serial.println("Returning to the main screen");
              break;
            } else {
              printSubMessage3(subChoice);
            }
          }
        }
        break;

      case 4:
        printSubMenu4();
        while (true) {
          if (Serial.available()) {
            int subChoice = Serial.read() - '0';
            Serial.println(subChoice);
            if (subChoice == 3) {
              Serial.println("Returning to the main screen");
              analogWrite(ledPin, 0);
              analogWrite(ledPin + 1, 0);
              analogWrite(ledPin + 2, 0);
              break;
            } else {
              printSubMessage4(subChoice);
            }
          }
        }
        break;

      default: Serial.println("Invalid"); break;
    }
    printMenu();
  }
}

// Function to display a menu of options to the user
void printMenu() {
  Serial.println("Main Menu:");
  Serial.println("Select an option:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

void printSubMenu1() {
  Serial.println("SubMenu1:");
  Serial.println("Select an option:");
  Serial.println("1.1. Sensors Sampling Interval");
  Serial.println("1.2. Ultrasonic Alert Threshold");
  Serial.println("1.3. LDR Alert Threshold");
  Serial.println("1.4. Back");
}

void printSubMenu2() {
  Serial.println("SubMenu2:");
  Serial.println("Select an option:");
  Serial.println("2.1. Yes");
  Serial.println("2.2. No");
}

void printSubMenu3() {
  Serial.println("SubMenu3:");
  Serial.println("Select an option:");
  Serial.println("3.1. Current Sensor Readings");
  Serial.println("3.2. Current Sensor Settings");
  Serial.println("3.3. Display Logged Data");
  Serial.println("3.4. Back");
}

void printSubMenu4() {
  Serial.println("SubMenu4:");
  Serial.println("Select an option:");
  Serial.println("4.1. Manual Color Control");
  Serial.println("4.2. LED : Toggle Automatic ON/OFF");
  Serial.println("4.3. Back");
}

// Function to print different messages based on the user's selection
void printMessage(int option) {
  Serial.print("\nPrinted message: ");
  switch (option) {
    case 1:
      Serial.println("Option 1");
      break;
    case 2:
      Serial.println("Option 2 selected\n");
      break;
    case 3:
      // Prints a placeholder for a custom message for option 3
      Serial.println("Custom message\n");
      break;
    default:
      // Handles any choices outside the defined options
      Serial.println("The selected option is outside the scope of the menu\n");
  }
}

void printSubMessage1(int subOption) {
  int sensorSamplingRate;
  int ultrasonicLimitSensor;
  int sensorBrightnessLimit;
  Serial.print("\nPrinted message: ");
  Serial.print(subOption);
  switch (subOption) {
    case 1:
      Serial.println("Option 1");
      sensorSamplingRate = getSamplingRate();    
      Serial.println("Sampling rate is: ");
      Serial.print(sensorSamplingRate);
      break;
    case 2:
      Serial.println("Option 2\n");
      ultrasonicLimitSensor = getUltrasonicLimit();
      Serial.println("Ultrasonic sensor limit is: ");
      Serial.print(ultrasonicLimitSensor);
      break;
    case 3:
      Serial.println("Option 3");
      sensorBrightnessLimit = getBrightnessLimit();
      Serial.println("Brightness limit is: ");
      Serial.print(sensorBrightnessLimit);
      printSubMenu1();
      break;
    case 4:
      break;
    default:
      // Handles any choices outside the defined options
      Serial.println("The selected option is outside the scope of the menu\n"); break;
  }
  printSubMenu1();
}

void printSubMessage2(int subOption) {
  Serial.print("\nPrinted message: ");
  switch (subOption) {
    case 1:
      Serial.println("Option 1");
      brightnessLimit = 0;
      samplingRate = 1;
      ultrasonicLimit = 0;

      // Save the default values in EEPROM
      EEPROM.put(address, brightnessLimit);
      EEPROM.put(samplingAddress, samplingRate);
      EEPROM.put(ultrasonicAddress, ultrasonicLimit);

      // Print the default values
      Serial.println("Limits have been reset.");
      Serial.println(brightnessLimit);
      Serial.println(samplingRate);
      Serial.println(ultrasonicLimit);
      printSubMenu2();

      break;
    case 2:
      break;
    default:
      // Handles any choices outside the defined options
      Serial.println("The selected option is outside the scope of the menu\n");
  }
}

void printSubMessage3(int subOption) {
  Serial.print("\nPrinted message: ");
  Serial.print(subOption);
  // if (subOption == 2) {
  //   Serial.println("Option 2 selected\n");
  //   displaySensorSettings();  //Function where we display the sensor settings 
  //   printSubMenu3();
  // }

  // if (subOption == 3) {
  //   Serial.println("Custom message\n");
  //   displayLastSensorReadings(); // Function where we print the last 10 readings of the sensors
  //   printSubMenu3();
  // }
  switch (subOption) {
    case 1:
      Serial.println("Press 9 to stop reading");

       startTime = millis();  // Save the start time
       interval = samplingRate * 1000;  // Interval between readings (in milliseconds)

      do {
        int brightnessValue = analogRead(sensorPin);
        int ultrasonicValue = readUltrasonicSensor();  // Function to read the ultrasonic sensor
        sensorReadings[currentReadingIndex] = brightnessValue;   // Save the value in the specific array
        sensorReadingsUltra[currentReadingIndex] = ultrasonicValue;
        currentReadingIndex = (currentReadingIndex + 1) % maxReadings;

        // Display the values read in the Serial Monitor
        Serial.print("Brightness value: ");
        Serial.println(brightnessValue);
        Serial.print("Ultrasonic value: ");
        Serial.println(ultrasonicValue);

        // Compare the values with the set limits
        if (brightnessValue > brightnessLimit) {
          Serial.println("Brightness limit exceeded");
        }

        if (ultrasonicValue > ultrasonicLimit) {
          Serial.println("Ultrasonic sensor limit exceeded");
        }

        // Check if the key 9 was pressed and the time interval has passed
        if (Serial.available() && Serial.read() == '9') {
          Serial.println("Reading stopped");
          break;  // Exit the loop if key 9 was pressed
        }

        // Wait until the time interval has passed
        while (millis() - startTime < interval) {
          // Wait without doing anything
        }

        // Reset the start time for the next interval
        startTime = millis();
      } while (true);

      printSubMenu3();
      break;
    case 2:
      Serial.println("Option 2 selected\n");
      displaySensorSettings();
      printSubMenu3();
      break;
    case 3:
      // Prints a placeholder for a custom message for option 3
      Serial.println("Custom message\n");
      displayLastSensorReadings();
      printSubMenu3();
      break;
    case 4:
      break;
    default:
      // Handles any choices outside the defined options
      Serial.println("The selected option is outside the scope of the menu\n");
  }
}

void printSubMessage4(int subOption) {
  Serial.print("\nPrinted message: ");
  String input;
  switch (subOption) {

    case 1:
      Serial.println("Option 1");
      Serial.println("Enter the color");
      while (!Serial.available()) {
      }
      Serial.println("After");
      input = Serial.readString();  //read the String from serial monitor

      Serial.println( input);

      if (input == "red") {
        setColor(255, 0, 0);   //255 0 0 for red green blue
      } else if (input == "green") {
        setColor(0, 255, 0);
      } else if (input == "blue") {
        setColor(0, 0, 255);
      } else {
        Serial.println(input);
      }
      printSubMenu4();
      break;
    case 2:
      Serial.println("Option 2 selected\n");
      printSubMenu4();
      break;
    case 3:
      break;
    default:
            // Handles any choices outside the defined options
      Serial.println("The selected option is outside the scope of the menu\n");
  }
}

int getBrightnessLimit() {
  // Read value from EEPROM
  EEPROM.get(address, brightnessLimit);

  Serial.print("Current brightness limit: ");
  Serial.println(brightnessLimit);
  Serial.println("Please enter a new brightness limit:");
  Serial.flush();
  while (true) {
    while (!Serial.available()) {
      // Wait until the user inputs something
    }

    // Read the value entered by the user
    brightnessLimit = Serial.parseInt();

    // Save the value to EEPROM
    EEPROM.put(address, brightnessLimit);

    // Display the confirmation message
    Serial.print("Selected brightness limit: ");
    Serial.println(brightnessLimit);

    // Exit the loop
    break;
  }

  return brightnessLimit;
}

int getSamplingRate() {
  // Read value from EEPROM
  EEPROM.get(samplingAddress, samplingRate);

  Serial.print("Current sampling rate: ");
  Serial.println(samplingRate);
  Serial.println("Please enter a new sampling rate between 1 and 10 seconds:");

  while (true) {
    while (!Serial.available()) {
      // Wait until the user inputs something
    }

    // Read the value entered by the user
    samplingRate = Serial.parseInt();

    // Check if the value is in the desired range
    if (samplingRate >= 1 && samplingRate <= 10) {
      // Save the value to EEPROM
      EEPROM.put(samplingAddress, samplingRate);
      break;  // Exit the loop if the value is valid
    } else {
      Serial.println("Invalid input. Please enter a value between 1 and 10 seconds:");
      Serial.print(samplingRate);
    }
  }

  Serial.print("Selected sampling rate: ");
  Serial.println(samplingRate);

  return samplingRate;
}

int getUltrasonicLimit() {
  // Read value from EEPROM
  EEPROM.get(ultrasonicAddress, ultrasonicLimit);

  Serial.print("Current ultrasonic limit: ");
  Serial.println(ultrasonicLimit);
  Serial.println("Please enter a new ultrasonic limit:");
  Serial.flush();
  while (true) {
    while (!Serial.available()) {
      // Wait until the user inputs something
    }

    // Read the value entered by the user
    ultrasonicLimit = Serial.parseInt();

    // Save the value to EEPROM
    EEPROM.put(ultrasonicAddress, ultrasonicLimit);

    // Display the confirmation message
    Serial.print("Selected ultrasonic limit: ");
    Serial.println(ultrasonicLimit);

    // Exit the loop
    break;
  }

  return ultrasonicLimit;
}

void displaySensorSettings() {
  // Display settings for the light sensor
  Serial.println("Sensor Settings:");

  // Display sampling rate for the light sensor
  int sensorSamplingRate = getSamplingRateSensor();
  Serial.print("Sensor Sampling Rate: ");
  Serial.print(sensorSamplingRate);
  Serial.println(" seconds");

  // Display the brightness limit for the light sensor
  int sensorBrightnessLimit = getBrightnessLimitSensor();
  Serial.print("Brightness limit: ");
  Serial.println(sensorBrightnessLimit);

  int ultrasonicSensorLimit = getUltrasonicSensorLimit();
  Serial.println("Ultrasonic sensor limit: ");
  Serial.println(ultrasonicSensorLimit);

  Serial.println("End of Sensor Settings");
}

int getSamplingRateSensor() {
  // Read value from EEPROM
  EEPROM.get(samplingAddress, samplingRate);

  return samplingRate;
}

int getBrightnessLimitSensor() {
  // Read value from EEPROM
  EEPROM.get(address, brightnessLimit);

  return brightnessLimit;
}

int getUltrasonicSensorLimit() {
  EEPROM.get(ultrasonicAddress, ultrasonicLimit);

  return ultrasonicLimit;
}

void displayLastSensorReadings() {
  Serial.println("Last 10 Sensor Readings:");

  for (int i = 0; i < maxReadings; ++i) {
    int index = (currentReadingIndex + i) % maxReadings;
    Serial.print("LDR Reading ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(sensorReadings[index]);

    Serial.print("Ultrasonic Reading ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(sensorReadingsUltra[index]);
  }

  Serial.println("End of Last Sensor Readings");
}

void setColor(int red, int green, int blue) {
  analogWrite(ledPin, green);
  analogWrite(ledPin + 1, blue);
  analogWrite(ledPin + 2, red);
}

long readUltrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  // Convert duration to distance
  long distance = duration * 0.034 / 2;

  return distance;
}


