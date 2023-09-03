#include "dos_api.h"

#include <stdio.h>

int main()
{
    dos_version_t ver = get_dos_version();
    printf("Current DOS version is %lf\n",
           ver.major + ver.minor / 100.0);
}
