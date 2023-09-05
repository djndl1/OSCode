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

int main()
{
    dos_version_t ver = dos_get_dos_version();
    printf("Current DOS version is %lf\n",
           ver.major + ver.minor / 100.0);

    printf("Current break flag is %d\n", dos_get_break_flag());
    dos_set_break_flag(true);
    printf("Current break flag is %d\n", dos_get_break_flag());
    dos_set_break_flag(false);
    printf("Current break flag is %d\n", dos_get_break_flag());

    printf("exiting\n");

    read_until_break();
}
