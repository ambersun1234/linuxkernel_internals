#include <stdio.h>

#include "xs.h"

int main(int argc, char *argv[])
{
    xs string = *xs_tmp("\n foobarbar \n\n\n");
    xs_trim(&string, "\n ");
    printf("[%s] : %2zu\n", xs_data(&string), xs_size(&string));

    xs prefix = *xs_tmp("((("), suffix = *xs_tmp(")))");
    xs_concat(&string, &prefix, &suffix);
    printf("[%s] : %2zu\n", xs_data(&string), xs_size(&string));

	printf("\ntest cow copy on stack\n");
	xs test = *xs_tmp("");
	xs_cow_copy(&test, &string);
	printf("[%s] : %2zu\n", xs_data(&string), xs_size(&string));
	printf("[%s] : %2zu\n", xs_data(&test), xs_size(&test));
	printf("[%p] : [%p]\n", xs_data(&test), xs_data(&string));

	printf("\ntest cow copy on heap\n");
	xs string2 = *xs_tmp("abcdefghijklmnopqrstuv");
	xs test2 = *xs_tmp("");
	xs_cow_copy(&test2, &string2);
	printf("[%s] : %2zu\n", xs_data(&string2), xs_size(&string2));
	printf("[%s] : %2zu\n", xs_data(&test2), xs_size(&test2));
	printf("[%p] : [%p]\n", xs_data(&test2), xs_data(&string2));

	printf("\ntest cow copy on heap\n");
	xs string3 = *xs_tmp("cHbC2VnYvV3O0MMwC1GEVfWQUHozZUkicxkxEgP2ySifc7P5edWzuyPiOoyxYAmVs5ExtHMDlPUYveflnJqF3b9X9qWtQmrsHuUOrnGfLLyrRM23t5SsnGTv7ArtseIG4N4NBSm632jQJTdu11lcGJQZj5mFnQE0Zcc78J9cazKYLF0AYe4bLiUTXJKsMKhxcvqsIVBQvpedDViocbVgcatIRsqnFUtywx5DLlHjcBNpMNXyAf47gXa4NEctfc7UioLx");
	xs test3 = *xs_tmp("");
	xs_cow_copy(&test3, &string3);
	printf("[%s] : %2zu\n", xs_data(&string3), xs_size(&string3));
	printf("[%s] : %2zu\n", xs_data(&test3), xs_size(&test3));
	printf("[%p] : [%p]\n", xs_data(&test3), xs_data(&string3));
	printf("reference count: %d\n", xs_get_refcnt(&string3));

    return 0;
}
