#include "dos_api.h"

#ifdef __WATCOMC__
#include <i86.h>
#endif

#include <dos.h>

#include "subfuncs.h"

static union REGS create_in_regs(u8 subfunc_no)
{
    union REGS i_regs = { 0 };
    i_regs.h.ah = subfunc_no;

    return i_regs;
}

dos_version_t get_dos_version()
{
    union REGS i_regs = create_in_regs(SUBFUNC_GET_DOS_VERSION);
    union REGS o_regs = { 0 };
    intdos(&i_regs, &o_regs);

    dos_version_t ver = { 0 };
    ver.major = o_regs.h.al;
    ver.minor = o_regs.h.ah;

    return ver;
}
