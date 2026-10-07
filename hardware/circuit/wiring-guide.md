# Hardware Wiring Guide

## Component List

| Component         | Quantity | Specification                |
|-------------------|----------|------------------------------|
| Arduino Board     | 1        | Any version with native USB HID (Leonardo / Micro / Due recommended) |
| Flex Sensor       | 1        | Standard 2.2" or 4.5" flex / bend sensor |
| Potentiometer     | 1        | 10 kΩ rotary potentiometer   |
| Resistor          | 1        | 320 Ω (for flex sensor voltage divider) |
| Push Button       | 1        | Momentary tactile switch     |
| Breadboard        | 1        | Half-size or full-size       |
| Connecting Wires  | Several  | Male-to-male jumper wires    |

---

## Wiring Connections

### 1. Flex Sensor → Analog Pin A0

The flex sensor is used in a **voltage divider** configuration with a 320 Ω resistor.

```
5V ──────────────┬──────────────────
                 │
            [Flex Sensor]
                 │
                 ├──────── A0 (Arduino)
                 │
             [320 Ω]
                 │
GND ─────────────┴──────────────────
```

- One end of the flex sensor connects to **5V**
- The other end connects to both **A0** and the 320 Ω resistor
- The other end of the 320 Ω resistor connects to **GND**

> **Note:** When the flex sensor bends, its resistance increases, which causes the voltage at A0 to drop, producing a measurable analog signal.

---

### 2. Potentiometer → Analog Pin A1

```
5V ──── [Left Pin]
          [Wiper (Centre)] ──── A1 (Arduino)
GND ─── [Right Pin]
```

- Left outer pin → **5V**
- Right outer pin → **GND**
- Wiper (middle pin) → **A1**

The potentiometer acts as a variable voltage divider. Rotating it changes the voltage at A1, which maps to the Y-axis mouse movement (launch angle).

---

### 3. Push Button → Digital Pin 2

```
Digital Pin 2 ──── [Button Terminal A]
                       [Button Terminal B] ──── GND
```

- One terminal of the button → **Digital Pin 2**
- Other terminal → **GND**
- The code uses `INPUT_PULLUP`, so **no external pull-up resistor is needed**.

When the button is pressed, Pin 2 reads **LOW** → triggers a mouse left click (fires the bird).

---

## Quick Reference Table

| Component         | Arduino Pin | Notes                              |
|-------------------|-------------|------------------------------------|
| Flex Sensor       | A0          | Voltage divider with 320 Ω resistor|
| Potentiometer     | A1          | Wiper connected to A1              |
| Push Button       | D2          | Uses `INPUT_PULLUP`, no resistor   |
| Flex + Resistor   | 5V / GND    | Power for voltage divider          |
| Potentiometer     | 5V / GND    | Power rails                        |

---

## Required Arduino Board

> ⚠️ **Important:** The Arduino `Mouse` library requires **native USB HID support**.  
> Standard Arduino Uno **does NOT support** the Mouse library natively.

**Recommended boards:**
- Arduino **Leonardo** ✅ (recommended)
- Arduino **Micro** ✅
- Arduino **Due** ✅
- Arduino **Pro Micro** (3.3V/5V) ✅

---

## Physical Assembly Notes

The project uses a **cardboard frame** to house all components:

1. The **flex sensor** is mounted inside the slingshot's pull-trigger mechanism
   - When the rubber band trigger is pulled back, it flexes the sensor
2. The **potentiometer** is connected to the rotating part of the frame
   - Rotating the launcher arm adjusts the potentiometer value (angle)
3. The **push button** is mounted on the frame as the fire trigger
4. The **Arduino** and breadboard are housed inside the cardboard body
5. A **USB cable** connects the Arduino to the PC running Angry Birds
