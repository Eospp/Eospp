.intel_syntax noprefix

.macro restore_kernel_segments
    push rax
    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov es, eax
    mov gs, eax
    pop rax
.endm

.macro restore_user_segments
    pop rax
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
.endm

.macro save_context
    
    push rbp
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax

.endm

.macro restore_context
 
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rbp
.endm

.macro restore_context_light
    add rsp, 256
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop rbp
.endm
