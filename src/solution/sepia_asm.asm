section .rodata
    align 16
    LINE1: dd 0.131, 0.168, 0.189, 0

    align 16
    LINE2: dd 0.534, 0.686, 0.769, 0

    align 16
    LINE3: dd 0.272, 0.349, 0.393, 0

section .text
global sepia_asm

%define MAX_VALUE 255

crop:
    cmp ax, MAX_VALUE
    ja load_max
    ret
    load_max:
        xor rax, rax
        mov al, MAX_VALUE
        ret
    ret

%macro fill_part 1
    movzx r8d, byte[r11 + rsi + %1]
    cvtsi2ss xmm%1, r8d
    shufps xmm%1, xmm%1, 0x00
%endmacro

%macro load_part 1
    movaps xmm%1, xmm0
    shufps xmm%1, xmm0, (%1 - 1)
    xor rax, rax
    movd eax, xmm%1
    call crop
    mov byte[r11 + rsi + (%1 - 1)], al
%endmacro

;rdi - pointer on struct img
sepia_asm:

    mov r10, qword[rdi] ;width
    mov r11, qword[rdi + 8] ;height
    mov rax, r10
    mul r11 ;r10 - size (pixels)
    mov r10, rax
    xor rax, rax ;number of pixels
    .loop:
        dec r10
        push rax
        mov rsi, 3
        mul rsi ;number of bytes
        mov rsi, rax
        mov r11, [rdi + 16]

        fill_part 1
        fill_part 2
        fill_part 3

        mulps xmm0, [LINE1]
        mulps xmm1, [LINE2]
        mulps xmm2, [LINE3]
        addps xmm0, xmm1
        addps xmm0, xmm2

        cvtps2dq xmm0, xmm0

        load_part 1
        load_part 2
        load_part 3

        pop rax
        inc rax
        test r10, r10
        jnz .loop
    ret
