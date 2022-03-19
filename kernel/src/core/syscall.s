.intel_syntax noprefix
.include "src/core/store_ctx.s"

.macro create_syscall number
.global _syscall\number
_syscall\number:
    sti
   
    push \number
    jmp syscall_common_handler
.endm

create_syscall 0
create_syscall 1
create_syscall 2
create_syscall 3
create_syscall 4
create_syscall 5
create_syscall 6
create_syscall 7
create_syscall 8
create_syscall 9

syscall_common_handler:
    save_context

    restore_kernel_segments

    mov rdi,rsp

    call _syscall_handler

    restore_context

    add rsp,8

    iretq
    