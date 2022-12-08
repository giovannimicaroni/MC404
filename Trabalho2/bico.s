
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
        beq t2, t0, 2f
        mv a1, t1
        li a7, 18
        li a0, 1
        ecall
        addi t1, t1, 1
        j 1b
    2:
        la a1, barra_n
        li a7, 18
        li a0, 1
        ecall
    ret


.globl gets
gets:
    mv a1, a0
    mv t3, a0
    li t1, '\n'
    li t2, 0
    1:
        li a0, 0
        li a2, 1
        li a7, 17
        ecall
        lb t0, 0(a1)
        beq t0, t1, 2f
        addi a1, a1, 1
        j 1b
    2:
        sb t2, 0(a1)
        mv a0, t3
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
    mv t0, a2 #base em t0
    mv t1, a0 #numero em t1; buffer em a1 já
    mv a4, t1
    mv t4, a1
    mv a5, a1
    li t2, 10
    li t3, 0
    li t6, 0
    li t5, 0

    conta:
        divu a4, a4, t0
        beq a4, t3, 3f
        addi t5, t5, 1
        j conta
    3:
        add t4, t4, t5
    1:
        beq t4, a1, 4f
        rem t6, t1, t0
        blt t6, t2, menor_10
        addi t6, t6, -10
        addi t6, t6, 'a'
        j 2f

        menor_10:
            addi t6, t6, '0'

    2:
        sb t6, 0(t4)
        divu t1, t1, t0
        addi t4, t4, -1
        j 1b

    4:
        mv a0, a5
        ret





    

.globl sleep
sleep:
    addi sp, sp, -4
    sw ra, 0(sp)
    mv t0, a0 #tempo de sleep em t0
    jal get_time
    add t0, a0, t0 #tempo de sleep + tempo inicial em t0
    1:
        bgt a0, t0, 2f
        jal get_time
        j 1b
    2:
        lw ra, 0(sp)
        addi sp, sp, 4
        ret

.globl approx_sqrt
approx_sqrt:
    #a0 -> y a1 -> iteraçoes
    li t5, 0
    srli t1, a0, 1 #k em t1
    divu t2, a0, t1 #y/k em t2
    add t2, t2, t1 #k + y/k em t2
    srli t2, t2, 1 #k' = (k + y/k)/2 em t2
    1:
        beq t5, a1, 2f 
        divu t3, a0, t2 #y/k' em t3
        add t3, t3, t2 #k' + y/k' em t3
        srli t2, t3, 1 #(k' + y/k')/2 em t2
        addi t5, t5, 1
        j 1b
    
    2:
        mv a0, t2
        ret 


barra_n: .ascii "\n"




# srli t3, a0, 1 # k em t3
# divu t4, a0, t3 # y/k em t4
# add t5, t3, t4 # k + y/k em t5
# srli t6, t5, 1 # (k + y/k)/2 em t6
# li t0, 0

# 1:
# bge t0, a3, 2f
# divu t4, a0, t6 # y/k' em t4
# add t5, t6, t4 # k' + y/k' em t5
# srli t6, t5, 1 # (k' + y/k')/2 em t6
# addi t0, t0, 1
# j 1b

# 2:
# mv a5, t6 #devolve o valor final em a5
# ret