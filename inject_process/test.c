#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int a = 123;
	int b = 234;
	int pid = getpid();
	while(1) {
		printf("pid: %d, a: %d, b: %d\n", pid, a, b);
		sleep(1);
	}
}
