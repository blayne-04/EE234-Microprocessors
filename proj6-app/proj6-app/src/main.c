#include "Defs/Wrappers.h"

int main(void)
{
    int OPMODE = 3;

    switch(OPMODE)
    {
        case 1: req1();
        break;

        case 2: req2();
        break;

        case 3: req3();
        break;

        case 4: req4();
        break;

        case 5: req5();
        break;

        default: printf("Invalid OPMODE!\n");
        break;
    }
}