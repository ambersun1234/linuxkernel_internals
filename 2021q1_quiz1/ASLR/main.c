#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    if (fork() == 0) {
        char *str[] = {"./test", NULL, NULL};
		char *envp[] = {0};
        if (execve("./test", str, envp) < 0) {
			perror("error");
			exit(0);
		}
    }

    return 0;
}
