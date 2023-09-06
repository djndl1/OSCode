#ifndef BIOS_API_H_
#define BIOS_API_H_

#include "modern.h"

/**
 * @struct bios_keyboard_input_t
 *
 * keyboard input result from BIOS
 */
typedef struct _bios_kbd_input {
    /**
     * ASCII code of a keyboard input
     *
     * @memberof bios_keyboard_input_t
     */
    u8 ascii_code;

    /**
     * Scan code of a keyboard input
     *
     * @memberof bios_keyboard_input_t
     */
    u8 scan_code;
} bios_keyboard_input_t;

/**
 * @brief read a keyboard input directly
 *
 * @returns a keyboard input with the ascii code and the scan code
 */
bios_keyboard_input_t bios_read_keyboard_input();


#endif // BIOS_API_H_
