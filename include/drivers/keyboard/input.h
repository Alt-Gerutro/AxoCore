#ifndef _DRIVERS_IO_INPUT_AXOCORE_H
#define _DRIVERS_IO_INPUT_AXOCORE_H

#include <stdint.h>

#define REG_INPUT_DATA      0x60

typedef struct {
    uint16_t scancode;
    char *string;
} keyPair;

static uint8_t inputBuffer[4096];

static keyPair keys[] = {
    {0x1, "esc"}, {0x3B, "F1"}, {0x3C, "F2"}, {0x3D, "F3"}, {0x3E, "F4"}, {0x3F, "F5"}, {0x40, "F6"}, {0x41, "F7"}, {0x42, "F8"}, {0x43, "F9"}, {0x44, "F10"}, {0x57, "F11"}, {0x58, "F12"},

    {0x29, "`"}, {0x2, "1"}, {0x3, "2"}, {0x4, "3"}, {0x5, "4"}, {0x6, "5"}, {0x7, "6"}, {0x8, "7"}, {0x9, "8"}, {0xA, "9"}, {0xB, "0"}, {0x0C, "-"}, {0x0D, "="},
    {0x10, "q"}, {0x11, "w"}, {0x12, "e"}, {0x13, "r"}, {0x14, "t"}, {0x15, "y"}, {0x16, "u"}, {0x17, "i"}, {0x18, "o"}, {0x19, "p"}, {0x1A, "["}, {0x1B, "]"},
    {0x1E, "a"}, {0x1F, "s"}, {0x20, "d"}, {0x21, "f"}, {0x22, "g"}, {0x23, "h"}, {0x24, "j"}, {0x25, "k"}, {0x26, "l"}, {0x27, ";"}, {0x28, "\'"},
    {0x2C, "z"}, {0x2D, "x"}, {0x2E, "c"}, {0x2F, "v"}, {0x30, "b"}, {0x31, "n"}, {0x32, "m"}, {0x33, ","}, {0x34, "."}, {0x35, "/"},

    {0x0E, "backspace"}, {0x0F, "tab"}, {0x1C, "enter"}, {0x3A, "caps"},
    {0x2A, "L shift"}, {0x36, "R shift"},
    {0x1D, "ctrl"}, {0x5B, "meta"}, {0x38, "alt"},
    {0x39, " "}
};

char* find_key_by_scancode(uint8_t sc);
char get_char(uint16_t pos);
uint8_t get_attr(uint16_t pos);

void backspace();

void keyboard_init();

#endif // _DRIVERS_IO_INPUT_AXOCORE_H