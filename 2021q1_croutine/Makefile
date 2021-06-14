croutine: main.c futex.c chan.c
	gcc -ggdb -pthread -fPIE -pie -fsanitize=thread $< -o $@

.PHONY: clean

clean:
	rm -rf a.out croutine
