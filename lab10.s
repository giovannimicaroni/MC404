.set CONTROLECARRO,0xFFFF0100

.bss
.align 4
isr_stack:
.skip 1024
isr_stack_end: .skip 4

program_stack:
.skip 1024
program_stack_end:

.text
.align 4

int_handler:
  ###### Tratador de interrupções e syscalls ######
  csrrw sp, mscratch, sp # Troca sp com mscratch
  addi sp, sp, -64 # Aloca espaço na pilha
  sw a0, 0(sp) # Salva a0
  sw a1, 4(sp) # Salva a1
  sw a7, 8(sp)

  # <= Implemente o tratamento da sua syscall aqui 
  li t1, 10
  beq a7, t1, syscall_set_engine_and_steering

  lw a7, 8(sp)
  lw a1, 4(sp) # Recupera a1
  lw a0, 0(sp) # Recupera a0
  addi sp, sp, 64 # Desaloca espaço da pilha
  csrrw sp, mscratch, sp # Troca sp com mscratch novamente
  csrr t0, mepc  # carrega endereço de retorno (endereço 
                 # da instrução que invocou a syscall)
  addi t0, t0, 4 # soma 4 no endereço de retorno (para retornar após a ecall) 
  csrw mepc, t0  # armazena endereço de retorno de volta no mepc
  mret           # Recuperar o restante do contexto (pc <- mepc)
  
syscall_set_engine_and_steering:
  li t1, CONTROLECARRO
  li t0, 0
  li t2, 127
  bgt a1, t2, 1f
  li t2, -127
  blt a1, t2, 1f 
  li t2, 1
  bgt a0, t2, 1f
  li t2, -1
  blt a0, t2, 1f
  j 2f
  1:
    li a0, -1
    ret
  2:
  sb a0, 33(t1)
  sb a1, 32(t1)
  li a0, 0
  ret

.globl _start
_start:

  la t0, int_handler  # Carregar o endereço da rotina que tratará as interrupções
  csrw mtvec, t0      # (e syscalls) em no registrador MTVEC para configurar
                      # o vetor de interrupções.
  la sp, program_stack_end
  la t0, isr_stack_end
  csrw mscratch, t0
# Escreva aqui o código para mudar para modo de usuário e chamar a função 
# user_main (definida em outro arquivo). Lembre-se de inicializar a 
# pilha do usuário para que seu programa possa utilizá-la.
  csrr t1, mstatus # Update the mstatus.MPP
  li t2, ~0x1800 # field (bits 11 and 12)
  and t1, t1, t2 # with value 00 (U-mode)
  csrw mstatus, t1 

  la t0, user_main # Loads the user software
  csrw mepc, t0 # entry point into mepc
  mret # PC <= MEPC; mode <= MPP;



.globl logica_controle
logica_controle:
  # implemente aqui sua lógica de controle, utilizando apenas as 
  # syscalls definidas.
  li a0, 1
  li a1, -15
  li a7, 10
  ecall
