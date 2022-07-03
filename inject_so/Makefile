TARGET= inject test1 test2 s.so

default: ${TARGET}
inject: inject.c
	gcc -o $@ $^
test1: s.so test1.c
	gcc -g -o $@ $^ 
test2: test2.c
	gcc -g -o $@ $^ -ldl
s.so: s.c
	gcc -shared -fPIC -o $@ $^
	objdump -S $@ >$@.S

clean:
	rm -f ${TARGET} *.S *.so
