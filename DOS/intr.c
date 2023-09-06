#include "dos_api.h"

#include <stdio.h>
#include <stdbool.h>

void read_until_break()
{
    printf("Keyboard read test,\n");
    printf("Hit enter to break.\n");
    while (true) {
        u8 c = dos_read_char_without_echo();

        if (c == '\r') break;
        if (c == '\0') {
            u8 scan_code = dos_read_char_without_echo();
            printf("An extended key is pressed with a scan code of %d.\n",
                   scan_code);
        } else {
            printf("A non-extended key is pressed with an ASCII code of %d.\n",
                   c);
        }
    }
}

void read_kbd_until_break()
{
    printf("BIOS Keyboard read test,\n");
    printf("Hit enter to break.\n");
    while (true) {
        bios_keyboard_input_t input = bios_read_keyboard_input();

        if (input.ascii_code == '\r') break;

        printf("A key is pressed: ASCII %d, Scan %d\n",
               input.ascii_code, input.scan_code);
    }
}

int main()
{
    dos_version_t ver = dos_get_dos_version();
    printf("Current DOS version is %lf\n",
           ver.major + ver.minor / 100.0);

    printf("Current break flag is %d\n", dos_get_break_flag());
    dos_set_break_flag(true);
    printf("Current break flag is %d\n", dos_get_break_flag());

    read_kbd_until_break();
    read_until_break();

    dos_set_break_flag(false);
    printf("Current break flag is %d\n", dos_get_break_flag());
}
