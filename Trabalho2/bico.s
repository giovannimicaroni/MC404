
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
    #a0 -> array representando a imagem; a1 -> vetor de 3 posiçoes representando o filtro 1D
    li t0, 255 #tamanho da imagem é sempre 256
    mv t6, a0 #imagem em t6
    li t1, 0
    lb t3, 0(a1) #valor do filtro posiçao 0 em t3
    lb t4, 1(a1) #valor do filtro posiçao 1 em t4
    lb t5, 2(a1) #valor do filtro posiçao 2 em t5

    lbu a2, 0(t6) #carrega o valor do primeiro pixel
    addi sp, sp, -4
    sb a2, 0(sp) #salva o valor do primeiro pixel inalterado na pilha
    sb t1, 0(t6) #coloca preto no primeiro pixel
    addi t6, t6, 1
    addi t1, t1, 1

    pixeis_intermediarios:
        beq t1, t0, pixel_final
        lbu a4, 0(sp)#carrega o pixel a esquerda, da pilha (ultimo atual inalterado)
        mul a4, a4, t3 #multiplica o pixel a esquerda pelo primeiro valor do filtro

        lbu a2, 0(t6) #carrega o pixel atual
        sb a2, 0(sp) #salva o pixel atual inalterado
        mul a2, a2, t4 #multiplica o valor do primeiro pixel pelo valor do meio do filtro

        lbu a3, 1(t6) #carrega o pixel a direita
        mul a3, a3, t5 #multiplica o valor do segundo pixel pelo ultimo valor do filtro
        add a3, a3, a4 #soma o valor do pixel da direita com o da esquerda com o filtro aplicado
        add a2, a2, a3 #soma os valores obtidos aplicando o filtro

        bgt a2, t0, 1f
        blt a2, zero, 2f
        j guarda   
        
        1:
            li a2, 255
            j guarda
        2:
            li a2, 0

        guarda:    
        sb a2, 0(t6) #guarda o valor obtido no pixel atual
        addi t6, t6, 1
        addi t1, t1, 1
        j pixeis_intermediarios

    pixel_final:
        sb zero, 0(t6) #coloca preto no ultimo pixel
        addi sp, sp, 4 #desaloca o espaço usado da pilha
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
    mv a1, a0 #string em a1
    li t3, 0 
    li t4, 10
    li t6, '0'
    1:
        lb t2, 0(a1) #byte da string em t2
        addi a1, a1, 1 #vai pro proximo byte
        bne t2, t0, 2f #se o byte nao é espaço, pula
        j 1b
    2:
        li a5, '-'
        li a4, 0
        li t0, '0'
        li t1, '9'
        li t5, 0
        beq t2, a5, negativo

    3:
        beq t2, t5, 5f #se o byte é o \0, pula pra 5
        blt t2, t0, 4f #se o byte é menor que '0', pula para 4
        bgt t2, t1, 4f #se o byte é maior que '9', pula para 4
        mul t3, t3, t4 #multiplica o valor guardado por 10
        sub t2, t2, t6 #subtrai '0' do byte lido
        add t3, t3, t2 #adiciona o valor do byte lido menos '0' ao valor guardado
    4:
        lb t2, 0(a1)
        addi a1, a1, 1
        j 3b

    negativo:
        li a4, 1
        j 4b
    5:
        beq a4, zero, fim
        li t0, -1
        mul t3, t3, t0
        fim:
        mv a0, t3
        ret



.globl itoa
itoa:

    #a0 -> valor; a1 -> buffer; a2 ->base
    li t2, 10
    li t3, 0
    mv t0, a0 #valor inteiro em t0
    mv t4, a1 #buffer em t4
    mv a6, a1 #buffer em a6
    beq a2, t2, decimal #se a base for decimal, vai pro rótulo decimal
    j hexa

    decimal:
        beq t0, t3, inverte #se o resultado da divisao for 0, acabou e basta inverter a string
        remu t1, t0, t2 #resto da divisao do numero atual por 10 em t1
        addi t1, t1, '0' #soma '0' ao resto da divisao para ter o caracter do numero em t1
        sb t1, 0(t4) #guarda o caracter no buffer
        divu t0, t0, t2 #divide o numero atual por 10 e guarda nele mesmo
        addi t4, t4, 1 #soma um ao endereço do buffer
        j decimal

    hexa:
        li t2, 16
        li t5, 10
        1:
        beq t0, t3, inverte #se o resultado da divisao for 0, acabou e basta inverter a string
        remu t1, t0, t2 #resto da divisao do numero atual por 16 em t1
        blt t1, t5, 2f #se o resto da divisao for menor que 10, vai para 2f
        addi t1, t1, -10 #caso contrario, subtrai 10 do resto e soma com o caracter
        addi t1, t1, 'a' # 'a', para obter a letra representativa do valor em hexadecimal
        j 3f
        2:
        addi t1, t1, '0' #se o resto for menor que 10, basta somar o caracter '0'
        3:
        sb t1, 0(t4) #guarda o caracter no buffer
        divu t0, t0, t2 #divide o numero atual por 16 e guarda nele mesmo
        addi t4, t4, 1 #soma um ao endereço do buffer
        j 1b
    
    inverte:
        sub t1, t4, a1 #calcula quantos digitos foram adicionados a string
        srli t5, t1, 1 #divide esse numero de digitos por 2, que será o numero de trocas necessárias
        mv t6, t4
        li t3, 0
        addi t4, t4, -1
        4:
        beq t3, t5, cabou #se o numero de trocas necessarias tiver sido feito, acabou
        lb a4, 0(t4) #carrega o valor do final da array nao trocada em a4
        lb a5, 0(a1) #carrega o valor inicial da array nao trocada em a5
        sb a4, 0(a1) #guarda o valor final da array nao trocada no inicio dela
        sb a5, 0(t4) #guarda o valor inicial da array nao trocada no fim dela
        addi t4, t4, -1 #subtrai 1 do fim da array
        addi a1, a1, 1 #soma 1 ao inicio da array
        addi t3, t3, 1 #soma 1 ao numero de trocas
        j 4b

    cabou:
        li t0, 0
        sb t0, 0(t6)
        mv a0, a6 #move o valor inicial da array para a0
        ret

.globl sleep
sleep:
    addi sp, sp, -8
    sw ra, 0(sp)
    mv t0, a0 #tempo de sleep em t0
    jal get_time
    add t0, a0, t0 #tempo de sleep + tempo inicial em t0
    1:
        bgt a0, t0, 2f
        sw ra, 4(sp)
        jal get_time
        lw ra, 4(sp)
        j 1b
    2:
        lw ra, 0(sp)
        addi sp, sp, 8
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