#include "dos_api.h"

#ifdef __WATCOMC__
#include <i86.h>
#endif

#include <dos.h>

#include "subfuncs.h"
#include "funcs.h"

static union REGS create_in_regs(u8 func_no, u8 subfunc_no)
{
    union REGS i_regs = { 0 };
    i_regs.h.ah = func_no;
    i_regs.h.al = subfunc_no;

    return i_regs;
}

void set_break_flag(bool break_flag)
{
    union REGS i_regs = create_in_regs(FUNC_BREAK_FLAG, SUBFUNC_SET_BREAK_FLAG);
    i_regs.h.dl = break_flag;
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);
}

bool get_break_flag()
{
    union REGS i_regs = create_in_regs(FUNC_BREAK_FLAG, SUBFUNC_GET_BREAK_FLAG);
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);

    return o_regs.h.dl;
}

dos_version_t get_dos_version()
{
    union REGS i_regs = create_in_regs(FUNC_GET_DOS_VERSION, 0);
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);

    dos_version_t ver = { 0 };
    ver.major = o_regs.h.al;
    ver.minor = o_regs.h.ah;

    return ver;
}
