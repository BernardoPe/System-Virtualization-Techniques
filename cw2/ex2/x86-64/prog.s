        .section .rodata

data:   .byte  47, 101, 116,  99
        .byte  47, 111, 115,  45
        .byte 114, 101, 108, 101
        .byte  97, 115, 101,   0
        .byte  73,  83,  69,  76

        .text
        .globl _start
_start:
        movq $-100, %rdi
        leaq data(%rip), %rsi
        xorq %rdx, %rdx
        movq $257, %rax
        syscall

        movq %rax, %r15

        movq %r15, %rdi
        xorq %rsi, %rsi
        movq $2, %rdx
        movq $8, %rax
        syscall

        movq %rax, %r14

        xorq %rdi, %rdi
        movq %r14, %rsi
        movq $1, %rdx
        movq $2, %r10
        movq %r15, %r8
        xorq %r9, %r9
        movq $9, %rax
        syscall

        movq $1, %rdi
        movq %rax, %rsi
        movq %r14, %rdx
        movq $1, %rax
        syscall

        xorq %rdi, %rdi
        movq $231, %rax
        syscall

        .end
