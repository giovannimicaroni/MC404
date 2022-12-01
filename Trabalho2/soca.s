.set CONTROLECARRO,0xFFFF0100


.bss
.align 4
isr_stack:
.skip 1024
isr_stack_end: .skip 4

program_stack:
.skip 1024
program_stack_end: .skip 4




.text

#-------------------------ROTINA DE TRATAMENTO DE EXCEÇÕES E INTERRUPÇÕES POR SOFTWARE--------------------------------- 

int_handler:
    ###### Tratador de interrupções e syscalls ######

    #Salvando o contexto
    csrrw sp, mscratch, sp # Troca sp com mscratch
    addi sp, sp, -64 # Aloca espaço na pilha
    sw a0, 0(sp) # Salva a0
    sw a1, 4(sp) # Salva a1
    sw a7, 8(sp)

    #Tratamento das syscalls  
    li t1, 10
    beq a7, t1, syscall_set_motor
    li t1, 11
    beq a7, t1, syscall_set_handbreak
    li t1, 12
    beq a7, t1, syscall_read_sensors
    li t1, 13
    beq a7, t1, syscall_read_sensor_distance
    li t1, 15
    beq a7, t1, syscall_get_position
    li t1, 16
    beq a7, t1, syscall_get_rotation
    li t1, 17
    beq a7, t1, syscall_read
    li t1, 18
    beq a7, t1, syscall_write
    li t1, 19
    beq a7, t1, syscall_draw_line
    li t1, 20
    beq a7, t1, syscall_get_systime

    #Recupera o contexto
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

#-------------------------SYSCALLS--------------------------------------------------------- 

syscall_set_motor:
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

syscall_set_handbreak:
    li t1, CONTROLECARRO
    li t0, 0
    blt a0, t0, 1f
    li t0, 1
    bgt a0, t0, 1f
    j 2f
    1:
        ret
    2:
        sb a0, 34(t1)
        ret

#-------------------------START--------------------------------------------------------- 
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

    la t0, user_main # Loads the user software ***MUDAR O NOME PRA FUNÇAO CERTA
    csrw mepc, t0 # entry point into mepc
    mret # PC <= MEPC; mode <= MPP;

