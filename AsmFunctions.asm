section .text
global CountLongTitles
extern wcslen

CountLongTitles:
    push    rbx
    push    rsi
    push    rdi
    push    r12

    xor     eax, eax            ; результат = 0

    test    rdx, rdx
    jz      .end

    mov     rsi, rcx            ; rsi = titles[]
    mov     rbx, rdx            ; rbx = count
    mov     rdi, r8             ; rdi = minLength

    xor     r12, r12            ; i = 0

.loop:
    cmp     r12, rbx
    jge     .end

    mov     rcx, [rsi + r12*8]
    test    rcx, rcx
    jz      .next

    push    r12
    push    rsi
    push    rbx
    push    rdi

    call    wcslen

    pop     rdi
    pop     rbx
    pop     rsi
    pop     r12

    cmp     rax, rdi
    jle     .next

    inc     eax

.next:
    inc     r12
    jmp     .loop

.end:
    pop     r12
    pop     rdi
    pop     rsi
    pop     rbx
    ret