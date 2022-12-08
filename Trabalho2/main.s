
.text
.align 4
.globl main
main:
    # la t3, filtro
    # li t1, -1
    # sb t1, 0(t3)
    # sb zero, 1(t3)
    # li t1, 3
    # sb t1, 2(t3) 
    # mv a1, t3
    # la a0, imagem
    # jal filter_1d_image
    la a0, string
    jal atoi
    keklul:
    la a0, string 
    jal puts
    hahaha:
        j hahaha


.align 4
imagem: .ascii "3HmairfWOBeSrcSKMzoLag9X4X1sHQ87SOCuQGmGSkCQZm3ckUMhUwFHNRjgMvf0gPv5lX9GQjglNWnzRpHCPGRXuOJJgKJUauoKebOkkMbaAXkDqhtFBG0RilNFngHle0HPZkLxXuvv57SkoBFEffeCaPob9vgWjWWxdzVxI1YThRbLJS5uw5tV4lkMHgMUQuPjE7gls5T228RbvByUGF5q3Ra3q5UF90S4CBADOsG8zWoRKsEght0l21ltK6F0"

filtro: .skip 0x20

string: .ascii "21\0"