raiz: #precisa do valor do numero a ser lido em a6

srli t3, a6, 1 # k em t3
divu t4, a6, t3 # y/k em t4
add t5, t3, t4 # k + y/k em t5
srli t6, t5, 1 # (k + y/k)/2 em t6
li a3, 9
li a4, 0

1:
bge a4, a3, 2f
divu t4, a6, t6 # y/k' em t4
add t5, t6, t4 # k' + y/k' em t5
srli t6, t5, 1 # (k' + y/k')/2 em t6
addi a4, a4, 1
j 1b

2:
mv a5, t6 #devolve o valor final em a5
ret

string_em_int: #dado uma string que representa um numero em um endereço armazenado em a1, transforma em um valor numerico e devolve ele em a6
lb t1, (a1)
addi t1, t1, -48
li t2, 1000
mul t1, t1, t2
lb t2, 1(a1)
addi t2, t2, -48
li t3, 100
mul t2, t3, t2
add t1, t1, t2
lb t2, 2(a1)
addi t2, t2, -48
li t3, 10
mul t2, t3, t2
add t1, t1, t2
lb t2, 3(a1)
addi t2, t2, -48
add t1, t1, t2
mv a6, t1
ret

int_em_string: #dado um valor numerico armazenado em a6, transforma ele em um string e armazena o endereço dela em a3
mv t0, a5
la t5, output_address
li t6, 10
sb t6, 4(t5)
rem t1, t0, t6
addi t2, t1, 48
sb t2, 3(t5)
li t6, 100
rem t2, t0, t6
sub t3, t2, t1
li t6, 10
div t3, t3, t6
addi t3, t3, 48
sb t3, 2(t5)
li t6, 1000
rem t3, t0, t6
sub t4, t3, t2
li t6, 100
div t4, t4, t6
addi t4, t4, 48
sb t4, 1(t5)
sub t4, t0, t3
li t6, 1000
div t1, t4, t6
addi t1, t1, 48
sb t1, (t5)
mv a3, t5
ret

output_address: .skip 0x20

read_entrada:

li a0, 0 # file descriptor = 0 (stdin)
la a1, input_adress #  buffer
li a2, 5 # size (lendo apenas 1 byte, mas tamanho é variável)
li a7, 63 # syscall read (63)
ecall
ret
input_adress: .skip 0x10  # buffer


write_saida:
li a0, 1            # file descriptor = 1 (stdout)
la a1, output_address
li a2, 5           # size
li a7, 64           # syscall write (64)
ecall
ret

.globl _start
_start:

li t0, 0
li t1, 4


jal read_entrada
jal string_em_int
jal raiz
jal int_em_string
jal write_saida

jal read_entrada
jal string_em_int
jal raiz
jal int_em_string
jal write_saida

jal read_entrada
jal string_em_int
jal raiz
jal int_em_string
jal write_saida

jal read_entrada
jal string_em_int
jal raiz
jal int_em_string
jal write_saida


