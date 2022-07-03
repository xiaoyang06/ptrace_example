#include <time.h>
#include <stdio.h>
#include <unistd.h>

int aaa(int, int);

int main(int argc, char **argv) {
	int a = 1;
	int b = 2;
	int pid = getpid();
	while(1)
	{
		printf("[%s] pid: %d, func: %p, a: %d + b: %d => %d\n", argv[0], pid, &aaa, a, b, aaa(a,b));
		sleep(1);
	}
}
