
.text
.align 4
.globl main
main:
    li a0, 121
    li a1, 10
    jal approx_sqrt
    hahaha:


.align 4
buffer: .ascii "HELLO\0"