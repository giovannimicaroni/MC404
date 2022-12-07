
.text
.align 4
#---------------------------MOTORES--------------------------
.globl set_motor
set_motor: 
    li a7, 10
    ecall
    ret

.globl set_handbreak
set_handbreak:
    li a7, 11
    ecall
    ret

#------------------------SENSORES-----------------------------
.globl read_camera
read_camera:
    li a7, 12
    ecall
    ret

.globl read_sensor_distance
read_sensor_distance:
    li a7, 13
    ecall 
    ret

.globl get_position
get_position:
    li a7, 15
    ecall
    ret

.globl get_rotation
get_rotation:
    li a7, 16
    ecall
    ret

#----------------------------TIMER-----------------------------
.globl get_time
get_time:
    li a7, 20
    ecall
    ret

#---------------------PROCESSAMENTO DE IMAGEM------------------
.globl filter_1d_image
filter_1d_image:
    ret

.globl display_image
display_image:
    li a7, 19
    ecall
    ret

#---------------------------OUTRAS-----------------------------
.globl puts #a0 -> endereço do inicio da string
puts:
    mv t1, a0
    mv a1, a0
    li t0, 0
    li a2, 1
    1:
        lb t2, 0(t1)
        beq t2, t0, hahaahha
        mv a1, t1
        li a7, 18
        li a0, 1
        ecall
    kekw:
        addi t1, t1, 1
        j 1b
    hahaahha:
        la a1, barra_n
        li a7, 18
        li a0, 1
        ecall
        ret


.globl gets
gets:
    addi sp, sp, -4 
    sw a0, 0(sp)
    mv a1, a0
    li t1, '\n'
    li t2, 0
    1:
        li a0, 0
        li a2, 1
        li a7, 17
        ecall
        lb t0, 0(a1)
        addi a1, a1, 1
        beq t0, t1, 2f
        j 1b
    2:
        sb t2, 0(a1)
        ret


.globl atoi
atoi:
    li t0, ' '
    mv a1, a0
    li t3, 0
    li t4, 10
    li t6, '0'
    1:
        lb t2, 0(a1) 
        addi a1, a1, 1
        bne t2, t0, 2f
        j 1b
    2:
        li t0, '0'
        li t1, '9'
        li t5, 0
    3:
        beq t2, t5, 5f
        blt t2, t0, 4f
        bgt t2, t1, 4f
        mul t3, t3, t4
        sub t2, t2, t6
        add t3, t3, t2
    4:
        lb t2, 0(a1)
        addi a1, a1, 1
        j 3b
    
    5:
        mv a0, t3
        ret





.globl itoa
itoa:
    ret

.globl sleep
sleep:
    ret

.globl approx_sqrt
approx_sqrt:
    ret


barra_n: .ascii "\n"




