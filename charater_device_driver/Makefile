obj-m+=echo.o

.PHONY: all clean install run

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) -pthread user.c -o user

install: echo.ko
	@sudo insmod echo.ko

uninstall:
	@sudo rmmod echo.ko

run: user
	@sudo ./user

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm -f user
