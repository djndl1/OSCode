#ifndef DOS_API_H_
#define DOS_API_H_

#include "modern.h"

typedef struct dos_version {
    u8 major;
    u8 minor;
} dos_version_t;

#ifdef __cplusplus
extern "C" {
#endif

dos_version_t get_dos_version();

#ifdef __cpp
}
#endif

#endif // DOS_API_H_
