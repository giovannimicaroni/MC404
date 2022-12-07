
.text
.align 4
.globl main
main:
    la a0, buffer
    jal gets


.bss
.align 4
buffer: .skip 0x24 