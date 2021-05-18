#include "stdio.h"

extern "C" int add(int a, int b)
{
    printf("fake one\n");
    return 0;
}
