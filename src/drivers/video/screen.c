#include <drivers/video/screen.h>
#include <stdarg.h>
#include <sys/types.h>
#include <utils/byte_io.h>
#include <stdint.h>

uint8_t make_color(uint8_t background, uint8_t foreground) {
    return ((background << 4) | foreground);
}

void set_cursor(uint16_t position) {
    position /= 2;  /* Transform position to byte offset */

    outB(REG_VGA_CTRL, 14);                         /* Send msg 0b1110 to get higher byte of cursor */
    outB(REG_VGA_DATA, (uint8_t)(position >> 8));
    outB(REG_VGA_CTRL, 15);                         /* Send msg 0b1111 to get lower byte of cursor */
    outB(REG_VGA_DATA, (uint8_t)(position & 0xFF));
}

uint16_t get_cursor() {
    uint16_t hb, lb;

    outB(REG_VGA_CTRL, 14);
    hb = inB(REG_VGA_DATA);
    outB(REG_VGA_CTRL, 15);
    lb = inB(REG_VGA_DATA);

    return (((hb << 8) + lb) * 2);
}

void scroll_line() {
    uint8_t i = 1;
    uint16_t last_line;

    while (i < MAX_ROWS) {
        mem_copy(
            (uint8_t*)(VGA_ADDRESS + (MAX_COLS * i * 2)),
            (uint8_t*)(VGA_ADDRESS + (MAX_COLS * (i-1) * 2)),
            (MAX_COLS * 2)
        );
        i++;
    }

    last_line = (MAX_BYTE_OFFSET - MAX_COLS * 2);
    i = 0;

    while (i < MAX_COLS) {
        write(' ', make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE), (last_line + i * 2));
        i++;
    }

    set_cursor(last_line);
}

void write(char character, uint8_t attribute, uint16_t offset) {
    uint8_t* vga = (uint8_t*)VGA_ADDRESS;
    vga[offset] = character;
    vga[offset+1] = attribute;
}

void put_char(char character, uint8_t attribute) {
    uint16_t offset = get_cursor();

    if (character == '\n') {
        if ((offset / 2 / MAX_COLS) == (MAX_ROWS - 1)) scroll_line();
        else set_cursor((offset - offset % (MAX_COLS * 2)) + MAX_COLS * 2);
    } else {
        if (offset == MAX_BYTE_OFFSET) scroll_line();
        write(character, attribute, offset);
        set_cursor(offset + 2);
    }
}

void clear_screen() {
    for (uint16_t i = 0; i < MAX_BYTE_OFFSET; i += 2) {
        write(' ', VGA_BYTE_BLACK | VGA_BYTE_WHITE, i);
    }
}

char get_char(uint16_t position) {
    char c;
    position *= 2;

    uint8_t* vga = (uint8_t*)VGA_ADDRESS;
    c = vga[position];

    return c;
}

uint8_t get_attr(uint16_t position) {
    uint8_t attr;
    position *= 2;
    uint8_t* vga = (uint8_t*)VGA_ADDRESS;
    attr = vga[position+1];

    return attr;
}

void backspace() {
    uint16_t prev_symbol = get_cursor() - 2;
    if (prev_symbol == 0) return;

    write(' ', make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE), prev_symbol);
    set_cursor(prev_symbol);
}

void int_to_str(char* buffer, uint32_t bufferSize, int32_t num) {
    if (!buffer || bufferSize < 1) return;
    uint32_t i = 0;
    uint8_t is_negative = 0;
    
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (num < 0) is_negative = 1;
    else num = -num;

    while (num < 0) {
        uint32_t digit = -(num % 10);
        buffer[i++] = digit + '0';
        num /= 10;
    }

    if (is_negative) buffer[i++] = '-';

    buffer[i] = '\0';

    int32_t start = 0;
    int32_t end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void int_to_hex(char* buffer, uint32_t bufferSize, uint8_t isUppercase, int32_t num) {
    if (!buffer || bufferSize < 4) return;
    uint32_t i = 0;
    
    
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = 'x';
        buffer[2] = '0';
        buffer[3] = '\0';
        return;
    }
    
    if (!(num < 0)) num = -num;
    
    while (num < 0) {
        uint32_t index = -(num % 16);
        if (isUppercase) {
            buffer[i++] = hex_upper[index];
        } else {
            buffer[i++] = hex_lower[index];
        }
        num /= 16;
    }
    
    buffer[i++] = 'x';
    buffer[i++] = '0';
    buffer[i] = '\0';
    
    int32_t start = 0;
    int32_t end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void int_to_bin(char* buffer, uint32_t bufferSize, int32_t num) {
    if (!buffer || bufferSize < 4) return;
    uint32_t i = 0;
    
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = 'b';
        buffer[2] = '0';
        buffer[3] = '\0';
        return;
    }
    
    if (!(num < 0)) num = -num;
    
    while (num < 0) {
        uint32_t bit = -(num % 2);
        buffer[i++] = bit + '0';
        num /= 2;
    }
    
    buffer[i++] = 'b';
    buffer[i++] = '0';
    buffer[i] = '\0';
    
    int32_t start = 0;
    int32_t end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

/**
 * @brief Calculating maximum buffer size for number in hex and bin.
 * 
 * @param num Number to calculate buffer size.
 * @param base Base of needed buffer.
 * 
 * @note Special symbols(like -, null-terminator, 0b/0x) not counting.
 */
uint32_t count_buffer_size(int32_t num, uint8_t base) {
    if (num == 0) return 1;
    
    uint32_t count = 0;
    
    if (!(num < 0)) num = -num;
    
    while (num < 0) {
        num /= base;
        count++;
    }
    return count;
}

void vformat(char* buffer, uint32_t bufferSize, char* fmt, va_list args) {
    uint32_t i = 0; /* For index in fmt */
    uint32_t j = 0; /* For index in buffer */
    
    while (fmt[i] && j < bufferSize - 1) {
        if(fmt[i] != '%') {
            buffer[j++] = fmt[i++];
        } else {
            i++;    /* Go to next symbol(mode symbol) */
            
            /* FORMAT SPECIFIERS */
            switch (fmt[i]) {
                case 's': {
                    char* str = va_arg(args, char*);
                    while (*str && j < bufferSize - 1) {
                        buffer[j++] = *str++; /* Write symbol from 
                                               * formatted string to buffer 
                                               * and move pointers */
                    }
                    break;
                }
                case 'd': {
                    char buf[12];
                    int32_t num = va_arg(args, int);
                    int_to_str(buf, 12, num);
                    uint32_t k = 0;
                    while (buf[k] && j < bufferSize - 1) {
                        buffer[j++] = buf[k++];
                    }
                    break;
                }
                case 'x': {
                    int32_t num = va_arg(args, int);
                    uint32_t hex_l_bufferSize = count_buffer_size(num, 16) + 3;
                    char hex_l_buffer[hex_l_bufferSize];
                    int_to_hex(hex_l_buffer, hex_l_bufferSize, 0, num);
                    uint32_t k = 0;
                    while (hex_l_buffer[k] && j < bufferSize - 1) {
                        buffer[j++] = hex_l_buffer[k++];
                    }
                    break;
                }
                case 'X': {
                    int32_t num = va_arg(args, int);
                    uint32_t hex_u_bufferSize = count_buffer_size(num, 16) + 3;
                    char hex_u_buffer[hex_u_bufferSize];
                    int_to_hex(hex_u_buffer, hex_u_bufferSize, 1, num);
                    uint32_t k = 0;
                    while (hex_u_buffer[k] && j < bufferSize - 1) {
                        buffer[j++] = hex_u_buffer[k++];
                    }
                    break;
                }
                case 'b': {
                    int32_t num = va_arg(args, int);
                    uint32_t bin_bufferSize = count_buffer_size(num, 2) + 3;
                    char bin_buffer[bin_bufferSize];
                    int_to_bin(bin_buffer, bin_bufferSize, num);
                    uint32_t k = 0;
                    while (bin_buffer[k] && j < bufferSize - 1) {
                        buffer[j++] = bin_buffer[k++];
                    }
                    break;
                }
                default: {
                    if(j < bufferSize - 1) {
                        buffer[j++] = fmt[i];
                    }
                    break;
                }
            }
            i++;    /* Go to next symbol in format string */
        }
    }
    buffer[j++] = '\0'; /* Set null-terminator to end string */
}

void print_str(char* string, uint8_t attribute) {
    while(*string) {
        put_char(*string++, attribute);
    }
}

void print_fmt(char* fmt, ...) {
    uint32_t buf_size = 512;
    while (sizeof(fmt) >= buf_size - 1) {
        buf_size += 512;
    }
    char buf[buf_size];
    
    va_list args;
    va_start(args, fmt);
    vformat(buf, buf_size, fmt, args); /* Get formatted string */
    va_end(args);
    
    for (uint32_t i = 0; buf[i]; i++) {
        put_char(buf[i], make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE));
    }
}