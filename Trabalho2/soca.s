#----------------------------CONSTANTES-------------------------------------

.set GPT, 0xFFFF0100
.set CONTROLECARRO, 0xFFFF0300
.set SERIAL, 0xFFFF0500
.set canvas, 0xFFFF0700

#-----------------------------VETORES E VARIÁVEIS------------------------------
.bss
.align 4
isr_stack:
.skip 1024
isr_stack_end: .skip 4


#-------------------------ROTINA DE TRATAMENTO DE EXCEÇÕES E INTERRUPÇÕES POR SOFTWARE--------------------------------- 
.text
int_handler:
    ###### Tratador de interrupções e Syscalls ######

    #Salvando o contexto
    csrrw sp, mscratch, sp # Troca sp com mscratch
    addi sp, sp, -64 # Aloca espaço na pilha
    sw a0, 0(sp) # Salva a0
    sw a1, 4(sp) # Salva a1
    sw a7, 8(sp)
    sw t1, 12(sp)

    #Tratamento das Syscalls  
    li t1, 10
    beq a7, t1, Syscall_set_motor
    li t1, 11
    beq a7, t1, Syscall_set_handbreak
    li t1, 12
    beq a7, t1, Syscall_read_sensors
    li t1, 13
    beq a7, t1, Syscall_read_sensor_distance
    li t1, 15
    beq a7, t1, Syscall_get_position
    li t1, 16
    beq a7, t1, Syscall_get_rotation
    li t1, 17
    beq a7, t1, Syscall_read
    li t1, 18
    beq a7, t1, Syscall_write
    # li t1, 19
    # beq a7, t1, Syscall_draw_line
    # li t1, 20
    # beq a7, t1, Syscall_get_systime

    recupera:
    #Recupera o contexto
    lw t1, 12(sp)
    lw a7, 8(sp)
    lw a1, 4(sp) # Recupera a1
    lw a0, 0(sp) # Recupera a0
    addi sp, sp, 64 # Desaloca espaço da pilha
    csrrw sp, mscratch, sp # Troca sp com mscratch novamente
    csrr t0, mepc  # carrega endereço de retorno (endereço 
                    # da instrução que invocou a Syscall)
    addi t0, t0, 4 # soma 4 no endereço de retorno (para retornar após a ecall) 
    csrw mepc, t0  # armazena endereço de retorno de volta no mepc
    mret           # Recuperar o restante do contexto (pc <- mepc)

#-------------------------SYSCALLS--------------------------------------------------------- 

Syscall_set_motor:
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
    sw a0, 0(sp)
    j recupera
    2:
    sb a0, 33(t1)
    sb a1, 32(t1)
    li a0, 0
    sw a0, 0(sp)
    j recupera

Syscall_set_handbreak:
    li t1, CONTROLECARRO
    li t0, 0
    blt a0, t0, 1f
    li t0, 1
    bgt a0, t0, 1f
    j 2f
    1:
        j recupera 
    2:
        sb a0, 34(t1)
        j recupera

Syscall_read_sensors:
    li t1, CONTROLECARRO
    li t0, 1
    sb t0, 1(t1)
    li t0, 0
    1:
        lb t2, 1(t1)
        beq t2, t0, 2f
        j 1b
    2:
        li t0, 0
        li t2, 256
        addi t4, t1, 0x24
        3:
        lb t3, 0(t4)
        sb t3, 0(a0)
        addi a0, a0, 1
        addi t4, t4, 1
        addi t0, t0, 1
        blt t0, t2, 3b
        j recupera



Syscall_read_sensor_distance:
    li t1, CONTROLECARRO
    li t0, 2
    sb t0, 2(t1)
    li t0, 0
    1:
        lb t2, 2(t1)
        beq t2, t0, 2f
        j 1b
    2:
        lw a0, 28(t1)
        sw a0, 0(sp)
        j recupera

Syscall_get_position:
    li t1, CONTROLECARRO
    li t0, 1
    sb t0, 0(t1)
    li t0, 0
    1:
        lb t2, 0(t1)
        beq t2, t0, 2f
        j 1b
    2:
        lw t0, 16(t1)
        sw t0, 0(a0)
        lw t0, 20(t1)
        sw t0, 0(a1)
        lw t0, 24(t1)
        sw t0, 0(a2)
        j recupera

Syscall_get_rotation:
    li t1, CONTROLECARRO
    li t0, 1
    sb t0, 0(t1)
    li t0, 0
    1:
        lb t2, 0(t1)
        beq t2, t0, 2f
        j 1b
    2:
        lw t0, 4(t1)
        sw t0, 0(a0)
        lw t0, 8(t1)
        sw t0, 0(a1)
        lw t0, 12(t1)
        sw t0, 0(a2)
        j recupera

Syscall_read:
    li t1, SERIAL
    li t3, 0
    mv t4, a1
    4:
        beq t3, a2, 3f
        li t0, 1
        sb t0, 2(t1)
        li t0, 0
        1:
            lb t2, 2(t1)
            beq t2, t0, 2f
            j 1b
        2:
            lb t0, 3(t1)
            sb t0, 0(t4)
            addi t4, t4, 1
            addi t3, t3, 1
            j 4b


    3:
        sw a2, 0(sp)
        j recupera

Syscall_write:
    li t1, SERIAL
    li t3, 0
    mv t4, a1
    4:
        beq t3, a2, 3f
        li t0, 1
        lb t5, 0(t4)
        sb t5, 1(t1)
        sb t0, 0(t1)
        li t0, 0
        1:
            lb t2, 0(t1)
            beq t2, t0, 2f
            j 1b
        2:
            addi t4, t4, 1
            addi t3, t3, 1
            j 4b

    3:
        j recupera
    

# Syscall_draw_line:


#     ret

# Syscall_get_systime:


#     ret

#-------------------------START--------------------------------------------------------- 
.globl _start
_start:

    la t0, int_handler  # Carregar o endereço da rotina que tratará as interrupções
    csrw mtvec, t0      # (e Syscalls) em no registrador MTVEC para configurar
                        # o vetor de interrupções.
    li sp, 0x07FFFFFC
    la t0, isr_stack_end
    csrw mscratch, t0
    # Escreva aqui o código para mudar para modo de usuário e chamar a função 
    # user_main (definida em outro arquivo). Lembre-se de inicializar a 
    # pilha do usuário para que seu programa possa utilizá-la.
    csrr t1, mstatus # Update the mstatus.MPP
    li t2, ~0x1800 # field (bits 11 and 12)
    and t1, t1, t2 # with value 00 (U-mode)
    csrw mstatus, t1 

    la t0, main # Loads the user software ***MUDAR O NOME PRA FUNÇAO CERTA
    csrw mepc, t0 # entry point into mepc
    mret # PC <= MEPC; mode <= MPP;

