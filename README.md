
---

## Overview

The **VFD_Display** library provides an interface to control a VFD display using SPI communication. The library allows you to configure the display, control brightness, write characters and strings, and create custom characters.

---

## Features

- Initialize and configure the display (rows, columns, brightness).
- Write single characters or strings at specific positions.
- Clear the display.
- Create and display custom characters.
- Set display brightness dynamically.

---

## Funtions

### 1. **Constructor**

```cpp
VFD_Display(uint8_t dinPin, uint8_t clkPin, uint8_t csPin);
```

**Parameters**:
- `dinPin`: Data input pin.
- `clkPin`: Clock pin.
- `csPin`: Chip select pin.

Creates an instance of the VFD_Display class, specifying the pins connected to the display.

---

### 2. **Initialization**

```cpp
void begin(uint8_t numRows, uint8_t numCols, uint8_t brightnessLevel);
```

**Parameters**:
- `numRows`: Number of rows on the display.
- `numCols`: Number of columns on the display.
- `brightnessLevel`: Brightness level (range: `0x01` to `0xFF`).

Initializes the display by setting up the rows, columns, and brightness level.

---

### 3. **Set Brightness**

```cpp
void setBrightness(uint8_t level);
```

**Parameters**:
- `level`: Brightness level (range: `0x01` to `0xFF`).

Adjusts the brightness of the display dynamically.

---

### 4. **Clear the Display**

```cpp
void clear();
```

Clears all characters from the display.

---

### 5. **Write Characters**

#### Write a Single Character
```cpp
void writeChar(uint8_t row, uint8_t col, char chr);
```

**Parameters**:
- `row`: Row index (0-based).
- `col`: Column index (0-based).
- `chr`: Character to display.

Writes a single character at the specified position.

#### Write a String
```cpp
void writeString(uint8_t row, uint8_t col, const char *str);
void writeString(uint8_t row, uint8_t col, String str);
```

**Parameters**:
- `row`: Row index (0-based).
- `col`: Column index (0-based).
- `str`: String to display.

Writes a string starting at the specified position.

---

### 6. **Custom Characters**

#### Add a Custom Character
```cpp
bool addCustomChar(char symbol, uint8_t *bitmap);
```

**Parameters**:
- `symbol`: Symbol used to represent the custom character.
- `bitmap`: A 5-byte array defining the bitmap of the character.

**Returns**: `true` if the character was added successfully; `false` if the maximum number of custom characters (`16`) is reached.

#### Write a Custom Character
```cpp
void writeCustomChar(uint8_t row, uint8_t col, char symbol);
```

**Parameters**:
- `row`: Row index (0-based).
- `col`: Column index (0-based).
- `symbol`: Symbol representing the custom character.

Displays the custom character at the specified position.

---

## Example Usage

### Basic Initialization
```cpp
#include "VFD_Display.h"

VFD_Display vfd(2, 3, 4); // DIN on pin 2, CLK on pin 3, CS on pin 4

void setup() {
    vfd.begin(2, 16, 0x0F); // Initialize 2x16 display with medium brightness
    vfd.clear();            // Clear the display
    vfd.writeString(0, 0, "Hello, VFD!"); // Write text on the first row
}
```

### Custom Characters
```cpp
#include "VFD_Display.h"

VFD_Display vfd(2, 3, 4);

uint8_t smiley[5] = {0x0E, 0x11, 0x0E, 0x00, 0x00}; // Bitmap for a smiley face

void setup() {
    vfd.begin(2, 16, 0x0F);
    vfd.addCustomChar(':)', smiley); // Add custom smiley face
    vfd.writeCustomChar(0, 0, ':)'); // Display the smiley face
}
```

---

## Notes

1. Ensure the SPI pins (`DIN`, `CLK`, `CS`) are connected correctly to the VFD display.
2. The display supports a maximum of `16` custom characters.
3. Brightness levels range from `0x01` (dim) to `0xFF` (bright).
