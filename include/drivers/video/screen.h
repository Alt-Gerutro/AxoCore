#ifndef _DRIVERS_SCREEN_AXOCORE_H
#define _DRIVERS_SCREEN_AXOCORE_H

#include <stdarg.h>
#include <stdint.h>

#define NULL    ((void*)0)

#define VGA_ADDRESS     0xB8000
#define MAX_ROWS        25
#define MAX_COLS        80
#define MAX_CHARS       MAX_ROWS * MAX_COLS
#define MAX_BYTE_OFFSET MAX_CHARS * 2

#define REG_VGA_CTRL    0x3D4
#define REG_VGA_DATA    0x3D5

/*
 * Colors in bytes.
 *
 * 0x0F - black background, white symbol;
 * 0xA0 - lime background, black symbol.
 */
enum color_byte {
    VGA_BYTE_BLACK = 0x0,
    VGA_BYTE_DARK_BLUE = 0x1,
    VGA_BYTE_GREEN = 0x2,
    VGA_BYTE_DARK_CYAN = 0x3,
    VGA_BYTE_DARK_RED = 0x4,
    VGA_BYTE_PURPLE = 0x5,
    VGA_BYTE_BROWN = 0x6,
    VGA_BYTE_LIGHT_GREY = 0x7,
    VGA_BYTE_GREY = 0x8,
    VGA_BYTE_BLUE = 0x9,
    VGA_BYTE_LIME = 0xA,
    VGA_BYTE_CYAN = 0xB,
    VGA_BYTE_LIGHT_RED = 0xC,
    VGA_BYTE_MAGENTA = 0xD,
    VGA_BYTE_YELLOW = 0xE,
    VGA_BYTE_WHITE = 0xF
};

static const char hex_upper[16] =  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static const char hex_lower[16] =  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

/**
 * @brief Combines background and foreground colors into single color byte.
 * 
 * @param background Background color (4-bit value).
 * @param foreground Foreground color (4-bit value).
 * 
 * @return uint8_t Combined color byte where background occupies high nibble and foreground occupies low nibble.
 */
uint8_t make_color(uint8_t background, uint8_t foreground);

/**
 * @brief Sets the cursor position on the text screen.
 * 
 * @param position Linear cursor position (0-based index from top-left corner).
 * 
 * @note Each screen cell occupies 2 bytes (character + attribute),
 *       so the byte offset is twice the logical position index.
 */
void set_cursor(uint16_t position);

/**
 * @brief Retrieves the current cursor position as a byte offset.
 * 
 * @return uint16_t Byte offset from the top-left corner of the screen.
 * 
 * @note Each screen cell occupies 2 bytes (character + attribute),
 *       so the byte offset is twice the logical position index.
 */
uint16_t get_cursor();

/**
 * @brief Scrolls the entrie screen contents by one line.
 * 
 * The top line is discarded, all other lines move up by one position,
 * and the bottom line is cleared to empty (zero-terminators symbols).
 * The cursor position is setting to last line.
 */
void scroll_line();

/**
 * @brief Writes a character with specified attribute to a specific screen location.
 * 
 * @param character The character to write (ASCII value).
 * @param attribute Color/attribute byte for the character.
 * @param offset Byte offset from top-left corner of screen (must be even).
 * 
 * @note Each screen cell occupies 2 bytes (character + attribute),
 *       so the byte offset is twice the logical position index.
 */
void write(char character, uint8_t attribute, uint16_t offset);

/**
 * @brief Writes a character at the current cursor position and move it to next index.
 * 
 * @param character The character to write.
 * @param attribute Color/attribute byte for the character.
 * 
 * @note Performs the same operation as write() but uses the current cursor position
 *       as the destination and automatically advances the cursor afterwards.
 */
void put_char(char character, uint8_t attribute);

/**
 * @brief Clear the entire screen by filling every cell with space characters.
 * 
 * Each character cell on the screen is filled with the space character.
 * The cursor is reset to top-left corner (position 0).
 */
void clear_screen();

/** 
 * @brief Retrieves the character at the specified position on screen.
 * 
 * @param position Position from top-left corner of screen.
 * 
 * @return char Character at the specified screen position.
 */
char get_char(uint16_t position);

/**
 * @brief Retrieves the attribute at the specified position on screen.
 * 
 * @param position Position from top-left corner of screen.
 * 
 * @return uint8_t Color/attribute at the specified screen position.
 */
uint8_t get_attr(uint16_t position);

/**
 * @brief Deletes the character immediately before the current cursor position.
 * 
 * Replaces the previous character with a space and moves the cursor backward.
 * If the cursor is already at the position 0 of the screen,
 * the function returns without performing any action.
 */
void backspace();

/**
 * @brief Prints string without format specifiers at current cursor position.
 * 
 * @param string String to print.
 * @param attribute Color/attribute to print.
 */
void print_str(char* string, uint8_t attribute);

/**
 * @brief Formats string using variable arguments and supported format specifiers.
 * 
 * @param buffer Output buffer to store the formatted string.
 * @param bufferSize Maximum size of the output buffer (including null-terminator).
 * @param fmt Format string containing specifiers.
 * @param args Variable arguments list for the format specifies.
 * 
 * @note The resulting formatted string is null-terminated and truncated to fit within bufferSize.
 * @note Specifiers: %s(string), %c(char), %d(decimal), %x(hex).
 */
void vformat(char* buffer, uint32_t bufferSize, char* fmt, va_list args);

/**
 * @brief Prints a formatted string at the current cursor position.
 * 
 * @param fmt Format string containing specifiers.
 * @param ... Variable arguments corresponding to format specifiers.
 * 
 * @note Specifiers: %s(string), %c(char), %d(decimal), %x(hex).
 */
void print_fmt(char* fmt, ...);

#endif // _DRIVERS_SCREEN_AXOCORE_H