#include "dos_api.h"

#ifdef __WATCOMC__
#include <i86.h>
#endif

#include "common.h"

#include "subfuncs.h"
#include "funcs.h"

u8 dos_read_char_without_echo()
{
    union REGS i_regs = create_in_regs(FUNC_READ_CHAR_WITHOUT_ECHO, 0);
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);

    return o_regs.h.al;
}

void dos_set_break_flag(bool break_flag)
{
    union REGS i_regs = create_in_regs(FUNC_BREAK_FLAG, SUBFUNC_SET_BREAK_FLAG);
    i_regs.h.dl = break_flag;
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);
}

bool dos_get_break_flag()
{
    union REGS i_regs = create_in_regs(FUNC_BREAK_FLAG, SUBFUNC_GET_BREAK_FLAG);
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);

    return o_regs.h.dl;
}

dos_version_t dos_get_dos_version()
{
    union REGS i_regs = create_in_regs(FUNC_GET_DOS_VERSION, 0);
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);

    dos_version_t ver = { 0 };
    ver.major = o_regs.h.al;
    ver.minor = o_regs.h.ah;

    return ver;
}
