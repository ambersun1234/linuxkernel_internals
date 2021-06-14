#include <stdio.h>

int main(int argc, const char *argv[]) {
	register void *p asm("sp");
	printf("%p\n", p);

	return 0;
}
