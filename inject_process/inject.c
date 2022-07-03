#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/ptrace.h>

int main(int argc, char **argv)
{
	long ret;
	long data;
	pid_t pid;
	int *a, *b;
	long params[3];
	struct user_regs_struct regs;

	if(argc != 2) {
		printf("%s <pid>\n", argv[0]);
		return -1;

	}
	pid = atoi(argv[1]);

	ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	if(ret != 0)
	{
		printf("[ERROR] attach process %d failed %s\n",
				pid, strerror(errno));
		return -1;
	}

	while(1) 
	{
		ret = ptrace(PTRACE_GETREGS, pid, NULL, &regs);
		if(ret == 0) 
		{
			long ins = ptrace(PTRACE_PEEKTEXT, pid, regs.rip, NULL);
			if(regs.rip == 0x400659)
			{
				data = ptrace(PTRACE_PEEKTEXT, pid, regs.rbp - sizeof(long), NULL);
				a=(void *)&data + 4;
				b=(void *)&data;
				printf("[INFO] ptrace_get_data: data: %ld, a: %d, b: %d\n",
						data, *a, *b);
				*a = 112233;
				ret = ptrace(PTRACE_POKETEXT, pid, regs.rbp - sizeof(long), data);
				if(ret != 0)
				{
					printf("[ERROR] ptrace_set_data: data: %ld\n",
							data, strerror(errno));
					break;
				}
				printf("[INFO] ptrace_set_data: data: %ld, a: %d, b: %d\n",
						data, *a, *b);
				break;
			}
			ret =  ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
		}

	}

	ret = ptrace(PTRACE_DETACH, pid, NULL, NULL);
	if(ret != 0)
	{
		printf("[ERROR] detach process %d failed %s\n",
				pid, strerror(errno));
		return -1;
	}
}
