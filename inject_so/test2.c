#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int a = 1;
	int b = 2;
	int pid = getpid();

	int (*func)(int,int);
	void *handle;
	char *error;
	handle = dlopen("./s.so", RTLD_LAZY);
	if (!handle) {
		printf("dlopen so failed: %s\n", dlerror());
		return -1;
	}
	*(void **) (&func) = dlsym(handle, "aaa");
	if ((error = dlerror()) != NULL)  {
		printf("dlsym get function faild %s\n", dlerror());
		return -1;
	}

	while(1)
	{
		printf("[%s] pid: %d, func: %p, a: %d + b: %d => %d\n", argv[0], pid, func, a, b, (*func)(a,b));
		sleep(1);
	}
	return 0;
}
