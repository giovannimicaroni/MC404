.data
input_file: .asciz "imagem.pgm"
.bss
input_adress: .skip 0x10  # buffer
height: .skip 0x10
width: .skip 0x10
fd: .skip 0x10
barra_n: .skip 0x10

.text
open:
la a0, input_file    # endereço do caminho para o arquivo
li a1, 0             # flags (0: rdonly, 1: wronly, 2: rdwr)
li a2, 0             # modo
li a7, 1024          # syscall open 
ecall   
ret


setPixel:
li a7, 2200 # syscall setGSPixel (2200)
ecall
ret

read:
la t0, fd
lw a0, 0(t0)
la a1, input_adress #  buffer
li a7, 63 # syscall read (63)
ecall
ret

write:
li a0, 1
li a7, 64           # syscall write (64)
ecall
ret

setCanvasSize:
li a7, 2201         # syscall setCanvasSize (2201)
ecall
ret

# setScaling:
# li a0, 1            # scaling horizontal
# la a1, string       # scaling vertical
# li a7, 2202         # syscall setScaling (2202)
# ecall
# ret

.globl _start
.text
_start:
jal open
la t1, fd
sw a0, 0(t1)
la t6, barra_n
li t1, '\n'
sb t1, 0(t6)
li a2, 3 # size 
jal read
li t1, ' ' //comparar com o caracter para ver se leu espaço
li a2, 1 //ler um byte
li t3, 0 // largura lida
li t4, 10 // multiplicador

1:
jal read
lbu t2, 0(a1)
beq t1, t2, 2f
mul t3, t3, t4
addi t2, t2, -48
add t3, t3, t2
j 1b

2:
la t5, width
sw t3, 0(t5)
li a2, 1

li t1, '\n' //comparar com o caracter para ver se leu espaço
li t3, 0 // largura lida
li t4, 10 // multiplicador

3:
jal read
lbu t2, 0(a1)
beq t1, t2, 4f
mul t3, t3, t4
addi t2, t2, -48
add t3, t3, t2
j 3b

4:
la t5, height
sw t3, 0(t5)
li a2, 1
la a1, height
li a2, 4
jal read
lw t0, 0(t5) //height em t0
la t5, width
lw t1, 0(t5) //width em t1
mv a0, t1
mv a1, t0
jal setCanvasSize
li t2, 0
li t3, 0

loop_height:
    li t2, 0
    beq t0, t3, fora
    loop_width:
        beq t1, t2, 1f
        li a2, 1
        jal read
        lbu t4, 0(a1)
        mv t6, t4
        mv s0, t4
        slli t4, t4, 24
        slli t6, t6, 16
        slli s0, s0, 8
        add t4, t4, t6
        add t4, t4, s0
        addi t4, t4, 255
        mv a0, t2
        mv a1, t3
        mv a2, t4
        jal setPixel
        addi t2, t2, 1
        j loop_width 
1:
    addi t3, t3, 1
    j loop_height
fora: