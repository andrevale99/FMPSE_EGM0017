#ifndef LCD16X2_H
#define LCD16X2_H

#include <stm32f411xe.h>

#include <stdint.h>

#define WRITE 1
#define CMD 0

#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define SECOND_LINE 0xC0

#define ENTRY_MODE_SET 0x04
#define INCREMENT (ENTRY_MODE_SET | 0x02)     // I
#define DECREMENT (ENTRY_MODE_SET | 0x00)     // D
#define SHIFT_DISPLAY (ENTRY_MODE_SET | 0x01) // S

#define DISPLAY_CONTROL 0x08
#define DISPLAY_OFF (DISPLAY_CONTROL | 0x00)  // D
#define DISPLAY_ON (DISPLAY_CONTROL | 0x04)   // D
#define CURSOR_ON (DISPLAY_CONTROL | 0x02)    // C
#define BLINK_CURSOR (DISPLAY_CONTROL | 0x01) // B

#define CURSOR_SHIFT 0x10
#define DISPLAY_SHIFT (CURSOR_SHIFT | 0x08) // S
#define CURSOR_MOVE (CURSOR_SHIFT | 0x00)   // C
#define SHIFT_RIGHT (CURSOR_SHIFT | 0x04)   // R
#define SHIFT_LEFT (CURSOR_SHIFT | 0x00)    // L

#define FUNCTION_SET 0x20
#define BITS_8 (FUNCTION_SET | 0x00)    // DL
#define BITS_4 (FUNCTION_SET | 0x01)    // DL
#define LINES_1 (FUNCTION_SET | 0x00)   // N
#define LINES_2 (FUNCTION_SET | 0x08)   // N
#define FONT_5x8 (FUNCTION_SET | 0x00)  // F
#define FONT_5x10 (FUNCTION_SET | 0x04) // F

#define SET_DDRAM 0x80

typedef int8_t lcd16x2_err;

typedef struct
{
    void (*write)(uint8_t);
} lcd16x2_pin_t;

typedef struct
{
    lcd16x2_pin_t d4;
    lcd16x2_pin_t d5;
    lcd16x2_pin_t d6;
    lcd16x2_pin_t d7;

    lcd16x2_pin_t en;
    lcd16x2_pin_t rs;

    void (*delay_ms)(uint32_t);
} lcd16x2_handle;

lcd16x2_err lcd16x2_init_4bits(const lcd16x2_handle *handle);

lcd16x2_err lcd16x2_send_cmd(const lcd16x2_handle *handle, uint8_t cmd);

lcd16x2_err lcd16x2_send_data(const lcd16x2_handle *handle, uint8_t data);

void lcd16x2_write_string(const lcd16x2_handle *handle, const char *str, uint8_t size);

void pulse_enable(const lcd16x2_handle *handle);

#endif