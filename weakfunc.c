#include <stdio.h>

void weakfunc() __attribute__((weak));

void weakfunc()
{
	printf ("default weak function\n");

}
