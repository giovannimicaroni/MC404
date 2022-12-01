.set GPT,0xFFFF0100
.set MIDISYNTHESIZER,0xFFFF0300
.bss
.align 4
.globl _system_time
_system_time: .skip 64

isr_stack:
.skip 1024
isr_stack_end: .skip 4

program_stack:
.skip 1024
program_stack_end:

.text
.align 2
.globl play_note
play_note:
addi sp, sp, -4
sw t0, 0(sp)
li t0, MIDISYNTHESIZER
sb a0, 0(t0)
sh a1, 2(t0)
sb a2, 4(t0)
sb a3, 5(t0)
sh a4, 6(t0)
lw t0, 0(sp)
addi sp, sp, 4
ret

.globl main_isr
main_isr:
#salva contexto
csrrw sp, mscratch, sp
addi sp, sp, -16
sw t0, 0(sp)
sw t1, 4(sp)
sw t2, 8(sp)
sw t3, 12(sp)
#trata interrupçao
la t0, _system_time
lw t1, 0(t0)
li t3, 100
li t2, GPT
sw t3, 8(t2)
add t1, t1, t3
sw t1, 0(t0)
#recupera contexto
lw t3, 12(sp)
lw t2, 8(sp)
lw t1, 4(sp)
lw t0, 0(sp)
addi sp, sp, 16
csrrw sp, mscratch, sp
mret

.align 2
.globl _start
_start:
la t0, _system_time
li t1, 0
sw t1, 0(t0)  
la sp, program_stack_end
#Registra a ISR
la t0, main_isr
csrw mtvec, t0
#Configura o mscratch
la t0, isr_stack_end
csrw mscratch, t0
#Configura o GPT
li t1, 100
li t2, GPT
sw t1, 8(t2)
 # Habilita Interrupções Externas
csrr t1, mie
li t2, 0x800 
or t1, t1, t2
csrw mie, t1
 # Habilita Interrupções Global
csrr t1, mstatus 
ori t1, t1, 0x8 
csrw mstatus, t1
jal main






