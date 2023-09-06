#include "bios_api.h"

#include "common.h"

#include "funcs.h"
#include "subfuncs.h"

bios_keyboard_input_t bios_read_keyboard_input()
{
    union REGS i_regs = create_in_regs(BIOS_KEYBOARD_READ_CHAR, 0);
    union REGS o_regs = { 0 };
    raw_intr(0x16, &i_regs, &o_regs);

    bios_keyboard_input_t result = { 0 };

    result.ascii_code = o_regs.h.al;
    result.scan_code = o_regs.h.ah;

    return result;
}
