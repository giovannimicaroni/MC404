
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
.globl puts
puts:
    ret

.globl gets
gets:
    ret

.globl atoi
atoi:
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







