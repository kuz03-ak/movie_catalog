; ========================================================
; Функция CountLongTitles - подсчёт фильмов с длинными названиями
; ========================================================
; int CountLongTitles(const wchar_t* titles[], int count, int minLength)
; ========================================================

section .text
global CountLongTitles
extern wcslen

CountLongTitles:
    ; Пролог - сохраняем регистры
    push    rbx
    push    rsi
    push    rdi
    
    ; Обнуляем счётчик результата
    xor     eax, eax
    
    ; Если count == 0, сразу выходим
    test    rdx, rdx
    jz      .function_end
    
    ; Сохраняем параметры в сохраняемых регистрах
    mov     rsi, rcx        ; rsi = массив указателей titles
    mov     rbx, rdx        ; rbx = количество элементов count
    mov     rdi, r8         ; rdi = минимальная длина minLength
    
    ; Индекс i = 0
    xor     rcx, rcx
    
.loop_start:
    cmp     rcx, rbx
    jge     .function_end
    
    ; Получаем titles[rcx]
    mov     rdx, [rsi + rcx * 8]
    test    rdx, rdx
    jz      .continue_loop
    
    ; Сохраняем регистры перед вызовом wcslen
    push    rcx
    push    rsi
    push    rbx
    push    rdi
    
    ; Вызываем wcslen
    mov     rcx, rdx
    call    wcslen
    
    ; Восстанавливаем регистры
    pop     rdi
    pop     rbx
    pop     rsi
    pop     rcx
    
    ; Сравниваем длину (RAX) с minLength (RDI)
    cmp     rax, rdi
    jle     .continue_loop
    
    ; Увеличиваем счётчик результата
    inc     eax
    
.continue_loop:
    inc     rcx
    jmp     .loop_start
    
.function_end:
    ; Эпилог - восстанавливаем регистры
    pop     rdi
    pop     rsi
    pop     rbx
    ret