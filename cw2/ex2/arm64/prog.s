        .section .rodata

data:   .byte  47, 101, 116,  99
        .byte  47, 111, 115,  45
        .byte 114, 101, 108, 101
        .byte  97, 115, 101,   0
        .byte  73,  83,  69,  76

        .text
        .globl _start
        /*
        arch	syscall	return	arg0	arg1	arg2	arg3	arg4	arg5
        arm64	x8	x0	x0	x1	x2	x3	x4	x5
        x86_64	rax	rax	rdi	rsi	rdx	r10	r8	r9
        */
_start:
        mov x0, #-100
        ldr x1, =data
        mov x2, #0
        mov w8, #56 // sys_openat
        svc #0

        mov x21, x0

        mov x2, #2
        mov x1, #0
        mov x0, x21
        mov w8, #62 // lseek
        svc #0

        mov x20, x0

        mov x0, #0
        mov x5, #0
        mov x2, #1
        mov x3, #2
        mov x1, x20
        mov x4, x21
        mov w8, #222 // mmap
        svc #0

        mov x19, x0

        mov x1, x19
        mov x2, x20
        mov x0, #1
        mov w8, #64 // write
        svc #0

        mov x0, #0
        mov w8, #94 // exit_group
        svc #0

        .end
