/** @file */

#ifndef DOS_API_H_
#define DOS_API_H_

#include "modern.h"

/**
 * @typedef half_t
 * x86 Half word
 */
typedef u8 half_t;

/**
 * @typedef word_t
 * x86 word
 */
typedef u16 word_t;

/**
 * @typedef dword_t
 * x86 double word
 */
typedef u32 dword_t;

/**
 * @typedef qword_t
 * x86 quad word
 */
typedef u64 qword_t;

/**
 * @struct dos_version_t
 *
 * the DOS version number.
 * e.g. 3.01 results in 3 as the major version and 1 as the minor.
 */
typedef struct _dos_version {
    /**
     * Major version number
     *
     * @memberof dos_version_t
     */
    u8 major;

    /**
     * Minor version number
     *
     * @memberof dos_version_t
     */
    u8 minor;
} dos_version_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reads the `<Ctrl><Break>` flag
 *
 * This determines whether DOS should test for active `<Ctrl><C>` or `<Ctrl><Break>`
 * keys on each function call, or on character input/output calls
 * and `<Ctrl><Break>` trigger interrupts 23H
 *
 * @returns if `true`, test only during character input/output, else test on every function call
 */
bool get_break_flag();

/**
 * @brief Set the `<Ctrl><Break>` flag
 *
 * Sets and unsets the `<Ctrl><Break>` flag. This detennines whether DOS should test
 * for the activation of the `<Ctrl><C>` or `<Ctrl><Break>` keys on each DOS
 * function call or character input/output calls. `<Ctrl><C>` and `<Ctrl><Break>`
 * trigger interrupt 23H.
 *
 * @param break_flag `false` if test only during character input/output,
 *                    `true` if test on every function call
 */
void set_break_flag(bool break_flag);

/**
 * @brief returns the DOS version number
 *
 * @returns the DOS version in the form of @ref dos_version_t
 */
dos_version_t get_dos_version();

#ifdef __cpp
}
#endif

#endif // DOS_API_H_
