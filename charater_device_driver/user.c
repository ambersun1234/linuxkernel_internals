#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_LENGTH 256
#define DRIVER "/dev/echo"

#define MYR 20
#define MYC 10

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static char receive[BUFFER_LENGTH];

void* cdd(void *data) {
	int fd, ret;

	// opening device driver
	fd = open(DRIVER, O_RDWR); // open with read/write
	if (fd < 0) {
		perror("Failed to open device\n");
		exit(1);
	}

	ret = write(fd, data, strlen((char *)data));
	if (ret < 0) {
		perror("Failed to write message to device");
		exit(1);
	}

	// kernel module respond
	ret = read(fd, receive, BUFFER_LENGTH);
	if (ret < 0) {
		perror("Failed to read message from device\n");
		exit(1);
	}
	printf("Data: %s --> Received from kernel: %s\n", (char *)data, receive);
	return (void *)0;
}

void generate(char *data, int row, int col) {
	int r, c;

	for (r = 0; r < row; r++) {
		for (c = 0; c < col - 1; c++) {
			int letter = (rand() % 26);
			*((data + r * col) + c) = 'A' + letter;
		}
		*((data + r * col) + c) = '\0';
	}
}

int main() {
	srand(time(NULL));
	pthread_t mythread[MYR];
	char data[MYR][MYC];

	int as = ARRAY_SIZE(data), bs = ARRAY_SIZE(data[0]);
	generate(&data[0][0], as, bs);

	for (int i = 0; i < as; i++) {
		pthread_create(&mythread[i], NULL, (void *)cdd, data[i]);
	}
	for (int i = 0; i < as; i++) {
		pthread_join(mythread[i], NULL);
	}
	return 0;
}
