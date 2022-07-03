# inject_process

# desc
ptrace example change process variable

test.S
```
				printf("pid: %d, a: %d, b: %d\n", pid, a, b);
  400644:	8b 4d f8             	mov    -0x8(%rbp),%ecx
  400647:	8b 55 fc             	mov    -0x4(%rbp),%edx
  40064a:	8b 45 f4             	mov    -0xc(%rbp),%eax
  40064d:	89 c6                	mov    %eax,%esi
  40064f:	bf 08 07 40 00       	mov    $0x400708,%edi
  400654:	b8 00 00 00 00       	mov    $0x0,%eax
  400659:	e8 c2 fe ff ff       	callq  400520 <printf@plt>
				sleep(1);

```

a addr: rpb - 4  
b addr: rbp - 8    
change data when instruct addr is 0x400659

# output

```
#make 
gcc -o inject inject.c
gcc -g -o test test.c
objdump -S test >test.S

#./test 
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 123, b: 234
pid: 706138, a: 112233, b: 234
pid: 706138, a: 112233, b: 234
pid: 706138, a: 112233, b: 234

#./inject 706138
[INFO] ptrace_get_data: data: 528280977642, a: 123, b: 234
[INFO] ptrace_set_data: data: 482037064532202, a: 112233, b: 234
```
