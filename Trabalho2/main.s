
.text
.globl main
main:
    la a0, string
    jal puts
    alou:
        j alou

string: .ascii "123\n\0"