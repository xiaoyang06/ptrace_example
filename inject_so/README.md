# inject_so

# desc
ptrace example change dynamic link libraries variable

s.so.S
```
0000000000000579 <aaa>:
 579:	55                   	push   %rbp
 57a:	48 89 e5             	mov    %rsp,%rbp
 57d:	89 7d ec             	mov    %edi,-0x14(%rbp)
 580:	89 75 e8             	mov    %esi,-0x18(%rbp)
 583:	c7 45 fc e8 03 00 00 	movl   $0x3e8,-0x4(%rbp)  #change1
 58a:	c7 45 f8 64 00 00 00 	movl   $0x64,-0x8(%rbp)
 591:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
 598:	8b 55 ec             	mov    -0x14(%rbp),%edx
 59b:	8b 45 e8             	mov    -0x18(%rbp),%eax
 59e:	01 c2                	add    %eax,%edx          #change2
 5a0:	8b 45 fc             	mov    -0x4(%rbp),%eax
 5a3:	01 c2                	add    %eax,%edx
 5a5:	8b 45 f8             	mov    -0x8(%rbp),%eax
 5a8:	01 d0                	add    %edx,%eax
 5aa:	89 45 f4             	mov    %eax,-0xc(%rbp)
 5ad:	8b 45 f4             	mov    -0xc(%rbp),%eax
 5b0:	5d                   	pop    %rbp
 5b1:	c3                   	retq 
```

ins_change:  change so's execute code and always effect.   
stack_change: change process stack data and only effect once.    

# output

```
# make
gcc -o inject inject.c
gcc -shared -fPIC -o s.so s.c
objdump -S s.so >s.so.S
gcc -g -o test1 s.so test1.c 
gcc -g -o test2 test2.c -ldl

# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`
# ldd test1
	linux-vdso.so.1 (0x00007fff48a76000)
	s.so (0x00007fb2c7287000)
	libc.so.6 => /lib64/libc.so.6 (0x00007fb2c6ec2000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fb2c7489000)
# ./test1 
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 1103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 2203
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 2103
[./test1] pid: 708091, func: 0x4005f0, a: 1 + b: 2 => 2103

# ./inject 708091
[DEBUG] get_so_saddr: cmdbuf: 7fde22a2f000, hex: 7fde22a2f000
[INFO] pid: 708091, so_saddr: 0x7fde22a2f000
[INFO] ins_change: read rip: 7fde22a2f583, c: 1000
[INFO] ins_change: set rip: 7fde22a2f583, c: 2000
[INFO] stack_change: read rip: 7fde22a2f59e, d: 100
[INFO] stack_change: set rip: 7fde22a2f59e, d: 200


# ldd test2
	linux-vdso.so.1 (0x00007ffdde1f5000)
	libdl.so.2 => /lib64/libdl.so.2 (0x00007f0025d6b000)
	libc.so.6 => /lib64/libc.so.6 (0x00007f00259a6000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f0025f6f000)
# ./test2
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 1103
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 1103
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 1103
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 1103
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 1103
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 2203
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 2103
[./test2] pid: 708108, func: 0x7f747f0ce579, a: 1 + b: 2 => 2103

# ./inject 708108
[DEBUG] get_so_saddr: cmdbuf: 7f747f0ce000, hex: 7f747f0ce000
[INFO] pid: 708108, so_saddr: 0x7f747f0ce000
[INFO] ins_change: read rip: 7f747f0ce583, c: 1000
[INFO] ins_change: set rip: 7f747f0ce583, c: 2000
[INFO] stack_change: read rip: 7f747f0ce59e, d: 100
[INFO] stack_change: set rip: 7f747f0ce59e, d: 200
```

