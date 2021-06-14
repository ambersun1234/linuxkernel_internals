compile: main test

test: test.c
	gcc -no-pie -o $@ $<

main: main.c
	gcc -no-pie -o $@ $<

.PHONY: clean

clean:
	rm -rf main test
