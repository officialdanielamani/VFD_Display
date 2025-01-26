#ifndef VFD_DISPLAY_H
#define VFD_DISPLAY_H

#include <Arduino.h>

#define MAX_CUSTOM_CHARS 16  // Maximum number of custom characters

class VFD_Display {
private:
    uint8_t din;       // Data input pin
    uint8_t clk;       // Clock pin
    uint8_t cs;        // Chip select pin
    uint8_t rows;      // Number of rows
    uint8_t cols;      // Number of columns
    uint8_t brightness; // Brightness level (0x01 to 0xFF)

    struct CustomChar {
        uint8_t bitmap[5];  // Bitmap for the character
        char symbol;        // Symbol for lookup
    };

    CustomChar customChars[MAX_CUSTOM_CHARS];
    uint8_t customCharCount;

    void spi_write_data(uint8_t data);

public:
    // Constructor
    VFD_Display(uint8_t dinPin, uint8_t clkPin, uint8_t csPin);

    // Initialization methods
    void begin(uint8_t numRows, uint8_t numCols, uint8_t brightnessLevel);

    // Display control methods
    void setBrightness(uint8_t level);
    void clear();

    // Write data
    void writeChar(uint8_t row, uint8_t col, char chr);
    void writeString(uint8_t row, uint8_t col, const char *str); // For C-style strings
    void writeString(uint8_t row, uint8_t col, String str);      // For String objects


    // Custom character support
    bool addCustomChar(char symbol, uint8_t *bitmap);
    void writeCustomChar(uint8_t row, uint8_t col, char symbol);
};

#endif
