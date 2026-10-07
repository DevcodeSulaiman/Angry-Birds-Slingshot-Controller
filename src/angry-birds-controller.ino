/**
 * ============================================================
 *   Angry Birds Slingshot Controller
 * ============================================================
 *
 * Project:   Angry Birds Slingshot Controller
 * Authors:   Mohammed Talha (UE234036)
 *            Md Sulaiman Qamar (UE234035)
 *            Utkarsh Sharma (UE234070)
 *            Anuj Gupta (UE234010)
 * Guide:     Mr. Kuldeep Singh
 * Institute: UIET, Panjab University, Chandigarh
 * Duration:  27-05-2024 to 11-06-2024 (Summer Training)
 * 
 * Description:
 *   This Arduino sketch reads sensor data from a Flex Sensor
 *   and a Potentiometer, then converts the analog readings
 *   into mouse movements to control the Angry Birds game.
 *
 *   - Flex Sensor  → X-axis mouse movement (slingshot pull/force)
 *   - Potentiometer → Y-axis mouse movement (launch angle)
 *   - Push Button  → Left mouse click (triggers bird launch)
 *
 * Hardware Connections:
 *   - Flex Sensor:    Analog Pin A0 (with 320 Ω pull-down resistor)
 *   - Potentiometer:  Analog Pin A1
 *   - Push Button:    Digital Pin 2 (INPUT_PULLUP)
 *
 * Libraries Used:
 *   - Mouse.h (Arduino built-in Mouse Core Library)
 *
 * Board:  Arduino (any version with native USB HID support,
 *         e.g., Arduino Leonardo, Micro, or Due)
 *
 * ============================================================
 */

#include <Mouse.h>

// ─── Pin Definitions ─────────────────────────────────────────
const int FLEX_SENSOR_PIN   = A0;   // Flex sensor → X-axis (force/pull)
const int POT_PIN           = A1;   // Potentiometer → Y-axis (angle)
const int BUTTON_PIN        = 2;    // Push button → left mouse click (trigger)

// ─── Tuning Parameters ────────────────────────────────────────
const int   FLEX_THRESHOLD        = 15;   // Minimum flex change to trigger X movement
const int   FLEX_MOVE_AMOUNT      = 30;   // Pixels moved left when flex threshold is met
const int   Y_AVERAGE_SAMPLES     = 10;   // Number of samples for Y-axis averaging filter
const int   MOUSE_SPEED_DIVISOR   = 8;    // Divides potentiometer delta → smoother motion
const long  DEBOUNCE_DELAY        = 50;   // Button debounce delay in milliseconds

// ─── State Variables ─────────────────────────────────────────
int  prevFlexValue  = 0;          // Last recorded flex sensor value
int  prevPotValue   = 0;          // Last recorded potentiometer value

int  yReadings[10]  = {0};        // Circular buffer for Y-axis averaging
int  yReadIndex     = 0;          // Current index in the buffer
long yTotal         = 0;          // Running total for averaging
int  yAverage       = 0;          // Computed average for Y-axis

bool buttonPressed      = false;  // Tracks button state
long lastDebounceTime   = 0;      // Last time button state changed

// ─────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(9600);

    pinMode(BUTTON_PIN, INPUT_PULLUP);   // Button uses internal pull-up resistor

    // Initialize the Mouse library
    Mouse.begin();

    // Read initial sensor baselines
    prevFlexValue = analogRead(FLEX_SENSOR_PIN);
    prevPotValue  = analogRead(POT_PIN);

    // Pre-fill the Y-axis averaging buffer
    for (int i = 0; i < Y_AVERAGE_SAMPLES; i++) {
        yReadings[i] = prevPotValue;
        yTotal      += prevPotValue;
    }
    yAverage = prevPotValue;

    Serial.println("Angry Birds Slingshot Controller Ready!");
    Serial.println("Flex Sensor -> X axis | Potentiometer -> Y axis | Button -> Click");
}

// ─────────────────────────────────────────────────────────────
void loop() {
    // ── Read Sensors ──────────────────────────────────────────
    int currentFlexValue = analogRead(FLEX_SENSOR_PIN);
    int currentPotValue  = analogRead(POT_PIN);

    // ── Y-Axis: Potentiometer with Moving-Average Filter ──────
    yTotal              -= yReadings[yReadIndex];
    yReadings[yReadIndex] = currentPotValue;
    yTotal              += currentPotValue;
    yReadIndex           = (yReadIndex + 1) % Y_AVERAGE_SAMPLES;
    yAverage             = yTotal / Y_AVERAGE_SAMPLES;

    int potDelta  = yAverage - prevPotValue;
    int yMovement = potDelta / MOUSE_SPEED_DIVISOR;

    // ── X-Axis: Flex Sensor with Threshold Gate ───────────────
    int flexDelta = currentFlexValue - prevFlexValue;
    int xMovement = 0;

    if (abs(flexDelta) >= FLEX_THRESHOLD) {
        xMovement = -FLEX_MOVE_AMOUNT;
        Serial.print("Flex triggered! Delta: ");
        Serial.println(flexDelta);
    }

    // ── Apply Mouse Movement ──────────────────────────────────
    if (xMovement != 0 || yMovement != 0) {
        Mouse.move(xMovement, yMovement, 0);
    }

    // ── Button: Push Button → Left Mouse Click (Launch) ───────
    bool reading = (digitalRead(BUTTON_PIN) == LOW);

    if (reading != buttonPressed) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading && !buttonPressed) {
            Mouse.press(MOUSE_LEFT);
            buttonPressed = true;
            Serial.println("Button pressed -> Mouse LEFT CLICK (Launch!)");
        } else if (!reading && buttonPressed) {
            Mouse.release(MOUSE_LEFT);
            buttonPressed = false;
            Serial.println("Button released -> Mouse LEFT RELEASE");
        }
    }

    // ── Debug Output ──────────────────────────────────────────
    Serial.print("Flex: ");
    Serial.print(currentFlexValue);
    Serial.print(" | Pot: ");
    Serial.print(currentPotValue);
    Serial.print(" | Avg Y: ");
    Serial.print(yAverage);
    Serial.print(" | dX: ");
    Serial.print(xMovement);
    Serial.print(" | dY: ");
    Serial.println(yMovement);

    // ── Update Previous Values ────────────────────────────────
    prevFlexValue = currentFlexValue;
    prevPotValue  = yAverage;

    delay(10);
}
