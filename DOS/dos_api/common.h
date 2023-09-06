#ifndef COMMON_H_
#define COMMON_H_

#ifdef __WATCOMC__
#include <i86.h>
#endif

#include "modern.h"
#include <dos.h>


#ifdef USE_I386
#define raw_intr(ivec, in, out) (int386)(ivec, in, out)
#else
#define raw_intr(ivec, in, out) (int86)(ivec, in, out)
#endif

static inline union REGS create_in_regs(u8 func_no, u8 subfunc_no)
{
    union REGS i_regs = { 0 };
    i_regs.h.ah = func_no;
    i_regs.h.al = subfunc_no;

    return i_regs;
}

#endif // COMMON_H_
