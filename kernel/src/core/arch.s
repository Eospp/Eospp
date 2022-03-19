.intel_syntax noprefix


.global _arch_enable_sse

_arch_enable_sse:
    mov eax, 0x1
    cpuid
    test edx, 1<<25
    jz .no_sse

    xor rax, rax
    mov rax, cr0
    and ax, 0xFFFB  
    or ax, 0x2      
    mov cr0, rax
    mov rax, cr4
    or ax, 3 << 9   
    mov cr4, rax

    .no_sse:

    ret

