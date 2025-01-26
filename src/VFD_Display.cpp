#include "VFD_Display.h"

// Constructor
VFD_Display::VFD_Display(uint8_t dinPin, uint8_t clkPin, uint8_t csPin) {
    din = dinPin;
    clk = clkPin;
    cs = csPin;

    pinMode(din, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(cs, OUTPUT);

    customCharCount = 0;  // Initialize custom character count
}

// SPI data transmission
void VFD_Display::spi_write_data(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(clk, LOW);
        if (data & 0x01) {
            digitalWrite(din, HIGH);
        } else {
            digitalWrite(din, LOW);
        }
        data >>= 1;
        digitalWrite(clk, HIGH);
    }
}

// Initialize the VFD display
void VFD_Display::begin(uint8_t numRows, uint8_t numCols, uint8_t brightnessLevel) {
    rows = numRows;
    cols = numCols;
    brightness = brightnessLevel;

    // Set number of digits (rows * columns)
    digitalWrite(cs, LOW);
    spi_write_data(0xE0);  // Set digits command
    delayMicroseconds(5);
    spi_write_data((rows * cols) - 1);  // Total segments - 1
    digitalWrite(cs, HIGH);
    delayMicroseconds(5);

    // Set brightness
    setBrightness(brightness);

    // Clear the display
    clear();

    // Add refresh command to ensure display is active
    digitalWrite(cs, LOW);
    spi_write_data(0xE8);  // Refresh command
    digitalWrite(cs, HIGH);
    delayMicroseconds(5);
}


// Set the display brightness
void VFD_Display::setBrightness(uint8_t level) {
    brightness = level;
    digitalWrite(cs, LOW);
    spi_write_data(0xe4);  // Set brightness command
    delayMicroseconds(5);
    spi_write_data(brightness);
    digitalWrite(cs, HIGH);
    delayMicroseconds(5);
}

// Clear the display
void VFD_Display::clear() {
    for (uint8_t i = 0; i < rows * cols; i++) {
        digitalWrite(cs, LOW);
        spi_write_data(0x20 + i);  // Address each position
        spi_write_data(0x20);     // Write a space character (ASCII 0x20)
        digitalWrite(cs, HIGH);
    }
}

// Write a single character to the display
void VFD_Display::writeChar(uint8_t row, uint8_t col, char chr) {
    uint8_t position = row * cols + col;  // Calculate position based on row and column
    digitalWrite(cs, LOW);
    spi_write_data(0x20 + position);  // Set position
    spi_write_data(chr + 0x30);      // Write character
    digitalWrite(cs, HIGH);
    delayMicroseconds(5);
}

// Write a string to the display
void VFD_Display::writeString(uint8_t row, uint8_t col, const char *str) {
    uint8_t position = row * cols + col;  // Calculate starting position
    uint8_t maxChars = cols - col;       // Maximum characters that can fit on the row
    uint8_t charCount = 0;

    digitalWrite(cs, LOW);
    spi_write_data(0x20 + position);  // Set position
    while (*str && charCount < maxChars) {  // Write until end of string or maxChars reached
        spi_write_data(*str);  // Write each character
        str++;
        charCount++;
    }
    digitalWrite(cs, HIGH);
    delayMicroseconds(5);
}

void VFD_Display::writeString(uint8_t row, uint8_t col, String str) {
    writeString(row, col, str.c_str());  // Convert String to C-style string and reuse the other function
}


// Add a custom character
bool VFD_Display::addCustomChar(char symbol, uint8_t *bitmap) {
    if (customCharCount >= MAX_CUSTOM_CHARS) {
        return false;  // Max custom characters reached
    }
    customChars[customCharCount].symbol = symbol;
    memcpy(customChars[customCharCount].bitmap, bitmap, 5);  // Copy the bitmap
    customCharCount++;
    return true;
}

// Write a custom character to the display
void VFD_Display::writeCustomChar(uint8_t row, uint8_t col, char symbol) {
    for (uint8_t i = 0; i < customCharCount; i++) {
        if (customChars[i].symbol == symbol) {
            uint8_t position = row * cols + col;  // Calculate position
            uint8_t memoryIndex = i;             // Use the customCharCount index as memory index

            // Step 1: Write the custom bitmap to memory (0x40 + memoryIndex)
            digitalWrite(cs, LOW);
            spi_write_data(0x40 + memoryIndex);  // Set memory index
            for (uint8_t j = 0; j < 5; j++) {
                spi_write_data(customChars[i].bitmap[j]);  // Send bitmap data
            }
            digitalWrite(cs, HIGH);

            // Step 2: Assign the custom character to the display (0x20 + position)
            digitalWrite(cs, LOW);
            spi_write_data(0x20 + position);  // Set display position
            spi_write_data(memoryIndex);      // Assign custom character memory index
            digitalWrite(cs, HIGH);

            // Update the display
            digitalWrite(cs, LOW);
            spi_write_data(0xe8);  // Refresh command
            digitalWrite(cs, HIGH);

            return;
        }
    }
}
