TARGET= inject test

default: ${TARGET}
inject: inject.c
	gcc -o $@ $^
test: test.c
	gcc -g -o $@ $^
	objdump -S $@ >$@.S

clean:
	rm -f ${TARGET} *.S
