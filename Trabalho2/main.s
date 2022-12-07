
.text
.align 4
.globl main
main:
    la a0, buffer
    jal puts
    aaa:
        j aaa


.align 4
buffer: .ascii "HELLO\0"