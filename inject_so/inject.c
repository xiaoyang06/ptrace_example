#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/ptrace.h>

long hexstr2long(char *str)
{
	int i;
	int tmp;
	char ch;
	long sum = 0;
	for(i = 0; i < strlen(str); i ++) {
		ch = str[i];
		if(ch >= '0' && ch <= '9') tmp = ch - '0';
		else if(ch >= 'a' && ch <= 'f') tmp = ch - 'a' + 10;
		else if(ch >= 'A' && ch <= 'F') tmp = ch - 'A' + 10;
		else return -1;
		sum = (sum<<4) + tmp;
	}
	return sum;
}

long get_so_saddr(pid_t pid, char *soname)
{
	long saddr;
	char cmdbuf[1024];

	snprintf(cmdbuf, sizeof(cmdbuf), "cat /proc/%d/maps|grep xp|grep %s |awk -F '-' '{printf $1}'", pid, soname);
	FILE *f = popen(cmdbuf, "r");
	if(f == NULL) return -1;
	memset(cmdbuf, 0, sizeof(cmdbuf));
	fread(cmdbuf, sizeof(cmdbuf), sizeof(char), f);
	pclose(f);

	saddr = hexstr2long(cmdbuf);
	printf("[DEBUG] get_so_saddr: cmdbuf: %s, hex: %lx\n", cmdbuf, saddr);
	return saddr;
}


int main(int argc, char **argv)
{
	long ret;
	long data;
	pid_t pid;
	int *c, *d;
	long saddr;
	long params[3];
	struct user_regs_struct regs;

	if(argc != 2)
	{
		printf("%s <pid>\n", argv[0]);
		return -1;

	}
	pid = atoi(argv[1]);
	saddr = get_so_saddr(pid, "s.so");
	printf("[INFO] pid: %d, so_saddr: 0x%lx\n", pid, saddr);

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
			data = ptrace(PTRACE_PEEKTEXT, pid, regs.rip, NULL);
			//1. change instruct data(c)
			if(regs.rip-saddr == 0x583) {
				c = (void *)&data + 3;
				printf("[INFO] ins_change: read rip: %lx, c: %d\n", regs.rip, *c);
				*c = 2000;
				ret = ptrace(PTRACE_POKETEXT, pid, regs.rip, data);
				if(ret != 0)
				{
					printf("[ERROR] ptrace_set_data: data: %ld\n",
							data, strerror(errno));
					break;
				}
				printf("[INFO] ins_change: set rip: %lx, c: %d\n", regs.rip, *c);
			}
			//2. change stack data(d)
			if(regs.rip-saddr == 0x59e) {
				long data = ptrace(PTRACE_PEEKTEXT, pid, regs.rbp - sizeof(long), NULL);
				d=(void *)&data;
				printf("[INFO] stack_change: read rip: %lx, d: %d\n", regs.rip, *d);
				*d = 200;
				ret = ptrace(PTRACE_POKETEXT, pid, regs.rbp - sizeof(long), data);
				if(ret != 0)
				{
					printf("[ERROR] ptrace_set_data: data: %ld\n",
							data, strerror(errno));
					break;
				}
				printf("[INFO] stack_change: set rip: %lx, d: %d\n", regs.rip, *d);
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
