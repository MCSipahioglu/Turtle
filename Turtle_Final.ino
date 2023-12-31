/*
 * ME331 FALL2020 Term Project Group 7
 * Author: Cem Geçgel
 * Version: 1.20
 *
 * Created on 28.1.2021, 21:44
 */

// Debug
//#define READING
#define LOGGING
#define MOVEMENT
#define DEBUG

// L I B R A R I E S
// ~~~~~~~~~~~~~~~~~

// For SD Card
#include <SPI.h>
#include <SD.h>

// For Gyro
#include <Wire.h>
#include <MPU6050.h>

// C O N S T A N T S
// ~~~~~~~~~~~~~~~~~

// Physical
#define DISPLACEMENT_PER_SECOND 0.00019
#define DELAY_AFTER_SETUP 10

// Serial
#define FORWARD_SIGNAL 0xFF
#define STOP_SIGNAL 0x0
#define TURN_SIGNAL 0xFF
#define ANALOG_TO_CELSIUS 0.48828125

// Logical
#define STATE_VERTICAL 0
#define STATE_HORIZONTAL 1
#define STATE_ANGULAR 2
#define STATE_ERROR 3
#define STATE_DONE 4

// Electronical
// Left Wheel
#define PIN_DRIVER_AIN1 7
#define PIN_DRIVER_AIN2 6
#define PIN_DRIVER_APWM 5
// Right Wheel
#define PIN_DRIVER_BIN1 2
#define PIN_DRIVER_BIN2 4
#define PIN_DRIVER_BPWM 3

// Temperature Sensor
#define PIN_TEMPERATURE_SENSOR A0

// F I E L D S
// ~~~~~~~~~~~

// Set by the user.
float rowLength;
float stepSize;
float rowWidth;
int rowCount;
int turnsCW;

// State of the robot.
int row;
float displacement;
float sinceMeasurement;
float angle;
unsigned char state;
unsigned char aimedState;
int blink;
unsigned long elapsedTime;

// SD card
File currentFile;

// Gyro
MPU6050 mpu;
float yaw;

// E L E C T R O N I C S   I M P L E M E N T A T I O N
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Outputs the signal to the driver that is connected to the given pins.
 * The turn is counter-clockwise if the signal is negative. */
void driver(int signal, char in1, char in2, char pwm) {
#ifdef MOVEMENT
        // If the signal is negative turns counter-clockwise.
        if (signal < 0) {
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
                analogWrite(pwm, -signal);
        } else {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
                analogWrite(pwm, signal);
        }
#endif
}

/** Signals the wheels to rotate with the given intensity.
 * If the signals are negative they turn backwards. */
void wheels(int left, int right) {
        // For the left wheels backwards is clockwise.
        driver(-left, PIN_DRIVER_AIN1, PIN_DRIVER_AIN2, PIN_DRIVER_APWM);
        // For the right wheels backwards is counter-clockwise.
        driver(right, PIN_DRIVER_BIN1, PIN_DRIVER_BIN2, PIN_DRIVER_BPWM);
}

/** Returns the current temperature reading. */
float temperature() {
#ifdef LOGGING
        return analogRead(PIN_TEMPERATURE_SENSOR) * ANALOG_TO_CELSIUS;
#else
        return 0.0;
#endif
}

/** Loads the necessary pins. */
void setup() {
        Serial.begin(115200);
        // Set the pin mode for the on-board LED.
        pinMode(LED_BUILTIN, OUTPUT);
#ifdef MOVEMENT
        Serial.println("Movement active.");
        // Set the pin modes for the driver connections.
        pinMode(PIN_DRIVER_AIN1, OUTPUT);
        pinMode(PIN_DRIVER_AIN2, OUTPUT);
        pinMode(PIN_DRIVER_APWM, OUTPUT);
        pinMode(PIN_DRIVER_BIN1, OUTPUT);
        pinMode(PIN_DRIVER_BIN2, OUTPUT);
        pinMode(PIN_DRIVER_BPWM, OUTPUT);
        // Initialize gyro
        if(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
                error();
                return;
        }
        // Set the gyro.
        mpu.calibrateGyro();
        mpu.setThreshold(3);
#endif
        // Set the initial state.
        state = STATE_VERTICAL;
        // Default
        rowLength = 1.0;
        stepSize = 0.1;
        rowWidth = 0.1;
        rowCount = 10;
        turnsCW = -TURN_SIGNAL;
#ifdef LOGGING
        Serial.println("Logging active.");
        // Set up the SD card.
        // Initialize the SD library and read the input file.
        // If the library fails to initialize.
        if (!SD.begin()) {
                Serial.println("Failed to initialize SD library!");
                // Set the state to DONE.
                error();
                return;
        }
#endif
#ifdef READING
        Serial.println("Reading active.");
        // If there is not enough data given.
        if (!(currentFile = SD.open("input.bin")) {
                Serial.println("Failed to open the input.bin file!");
                // Set the state to DONE.
                error();
                return;
        }
        if (currentFile.available() != 20) {
                Serial.println("The input.bin file is not 20 bytes long!");
                // Set the state to DONE.
                error();
                return;
        }
        // Read the input bytes.
//        rowLength = readFloat();
//        stepSize = readFloat();
//        rowWidth = readFloat();
//        rowCount = readInt();
//        turnsCW = -readInt();
        // Close the file.
        currentFile.close();
#endif
#ifdef LOGGING
        // Open the output file.
        if (!(currentFile = SD.open("output.txt", FILE_WRITE))) {
                Serial.println("Failed to open the output file!");
                // Set the state to DONE if the file could not be opened.
                error();
                return;
        }
        // Write the data given by the user.
        currentFile.print("Row Length: ");
        currentFile.println(rowLength);
        currentFile.print("Step Size: ");
        currentFile.println(stepSize);
        currentFile.print("Row Width: ");
        currentFile.println(rowWidth);
        currentFile.print("Row Count: ");
        currentFile.println(rowCount);
        currentFile.print("Turns CW: ");
        currentFile.println(-turnsCW);
        // Close the file.
        currentFile.close();
#endif
        
#ifdef DELAY_AFTER_SETUP
        Serial.println("Delaying...");
        delay(1000 * DELAY_AFTER_SETUP);
#endif
        Serial.println("Values:");
        Serial.print("Row Length: ");
        Serial.println(rowLength);
        Serial.print("Step Size: ");
        Serial.println(stepSize);
        Serial.print("Row Width: ");
        Serial.println(rowWidth);
        Serial.print("Row Count: ");
        Serial.println(rowCount);
        Serial.print("Turns CW: ");
        Serial.println(-turnsCW);
}

/** Updates the yaw. */
void gyroUpdate() {
#ifdef MOVEMENT
        // Read normalized values
        Vector norm = mpu.readNormalizeGyro();
        // Calculate Pitch, Roll and Yaw
        yaw = yaw + norm.ZAxis * elapsedTime / 1000.0;
#endif
}

// E L E C T R O N I C S   I N T E R F A C E
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Sets the signals so the wheels make the robot go forwards. */
void forward() {
        wheels(FORWARD_SIGNAL, FORWARD_SIGNAL);
}

/** Sets the signals so the wheels stop turning. */
void stop() {
        wheels(STOP_SIGNAL, STOP_SIGNAL);
}

/** Sets the signals so the wheels make the robot turn.
 * If the signal given is positive the direction is clockwise. */
void turn(int cw) {
        wheels(cw, -cw);
}

/** Stores the current temperature to the SD card. */
void storeTemperature() {
#ifdef LOGGING
        // Open the output file.
        if (!(currentFile = SD.open("output.txt", FILE_WRITE))) {
                Serial.println("Failed to open the output file!");
                // Set the state to DONE if the file could not be opened.
                error();
                return;
        }
        // Write the temperature.
        currentFile.println(temperature());
        // Close the file.
        currentFile.close();
#endif
}

// L O G I C
// ~~~~~~~~~

/** Makes sure the given angle is between 0 and 2Pi. */
float zeroTo2Pi(float a) {
        float result = a;
        while (result < 0.0)
                result += 360.0;
        while (result >= 360.0)
                result -= 360.0;
        return result;
}

/** Returns the absolute value of the given float. */
float absolute(float a) {
        return a < 0.0 ? -a : a;
}

/** Signals for forward movement and records the displacement.
 * The direction of movement is ignored. */
void forwardMovement() {
        // Move by a tick.
        forward();
        float d = DISPLACEMENT_PER_SECOND * elapsedTime;
        // Record the displacement.
        displacement += d;
        sinceMeasurement += d;
}

/** Updates the vertical state. */
void verticalStateUpdate() {
        forwardMovement();
        // Check for the end of the row.
        Serial.println("-----------Disp:");
        Serial.println(displacement);
        Serial.println(sinceMeasurement);
        if (displacement >= rowLength) {
                Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                Serial.print("End of the row ");
                Serial.println(row + 1);
                Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                // Change the state to rotation.
                state = STATE_ANGULAR;
                // Revert the turning direction.
                turnsCW = -turnsCW;
                // If the previous row was the last one.
                if (++row == rowCount) {
                        // Change the state to done.
                        itIsDone();
                        return;
                }
                // Prepare for the rotation state.
                aimedState = STATE_HORIZONTAL;
                angle = yaw;
                Serial.print("Angle: ");
                Serial.print(angle);
                displacement = 0.0;
        // Check for the measurement spot.
        } else if (sinceMeasurement >= stepSize) {
                stop();
                // Store the temperature.
                storeTemperature();
                // Reset the displacement since the last measurement.
                sinceMeasurement = 0.0;
        }
}

/** Updates the horizontal state. */
void horizontalStateUpdate() {
        forwardMovement();
        // Check for the start of the row.
        if (displacement >= rowWidth) {
                Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                Serial.print("End of changing row ");
                Serial.println(row);
                Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                // Change the state to rotation.
                state = STATE_ANGULAR;
                // Prepare for the rotation state.
                aimedState = STATE_VERTICAL;
                angle = yaw;
                Serial.print("Angle: ");
                Serial.print(angle);
                displacement = 0.0;
                sinceMeasurement = 0.0;
        }
}

/** Updates the angular state. */
void angularStateUpdate() {
        // Turn by a tick.
        turn(turnsCW);
        Serial.print("Yaw: ");
        Serial.print(yaw);
        float difference = absolute(angle - yaw);
        Serial.print("Difference: ");
        Serial.println(difference);
        Serial.println("______________________");
        // If the robot turned a right angle.
        if (difference >= 90.0)
                // Change to the next state.
                state = aimedState;
}

/** Changes the state to ERROR and signals the wheels to stop. */
void error() {
        state = STATE_ERROR;
        // Prepare for blinking.
        blink = HIGH;
        // Stop the wheels.
        stop();
}

/** Changes the state to DONE and signals the wheels to stop. */
void itIsDone() {
        state = STATE_DONE;
        // Stop the wheels.
        stop();
}

/** Updates the state of the robot. */
void loop() {
        unsigned long timer = millis();
        gyroUpdate();
        // Break up the logic into different functions to increase readability.
        switch (state) {
        case STATE_VERTICAL:
                verticalStateUpdate();
                break;
        case STATE_HORIZONTAL:
                horizontalStateUpdate();
                break;
        case STATE_ANGULAR:
                angularStateUpdate();
                break;
        case STATE_ERROR:
                // Blink the on-board LED.
                digitalWrite(LED_BUILTIN, blink);
                if (blink == HIGH)
                        blink = LOW;
                else
                        blink = HIGH;
                break;
        case STATE_DONE:
                break;
        }
        // Wait for the correct tick rate.
        elapsedTime = millis() - timer;
}
