
string: .ascii "123\0"

.text
.globl _start
_start:
    la a0, string
    jal atoi
    alou:
