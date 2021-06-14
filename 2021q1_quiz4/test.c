#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>

bool lala(atomic_int *a, atomic_int *b) {
    bool result = atomic_compare_exchange_weak(a, b, (atomic_int)1);
    return result;
}

int main(int argc, const char *argv[]) {
    atomic_int a = 6;
    atomic_int b = 3;
    bool result;

    printf("a: %d, b: %d\n", a, b);
    result = lala(&a, &b);
    printf("%d\n", result);
    printf("a: %d, b: %d\n", a, b);

    result = lala(&a, &b);
    printf("%d\n", result);
    printf("a: %d, b: %d\n", a, b);

    return 0;
}