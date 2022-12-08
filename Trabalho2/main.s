
.text
.align 4
.globl main
main:
    li a0, 123
    la a1, buffer
    li a2, 10
    jal itoa
    hahaha:


.align 4
buffer: .skip 0x20