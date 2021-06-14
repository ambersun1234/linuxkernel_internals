all: list.c
	gcc $<

SRCS=$(wildcard picture/*.dot)
OBJS=$(patsubst %.dot, %.png, $(SRCS))

pic: $(OBJS)

%.png: %.dot
	dot -T png $< > $@

.PHONY: clean

clean:
	rm -r *.out *.png
