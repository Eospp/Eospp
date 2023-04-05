[BITS 16]


jmp start

%include "io.asm"

start:
	;init stack sapce 4K
	xor ax,ax
	mov ss,ax 
	mov sp,0x4000H

	;set data segment,boot sector loaded to 0x7c00H
	mov ax,0x7C0H
	mov ds,ax 

	; Hide cursor
	mov ah,0x01H
	mov cx,0x2607H
	int 10H 

    ; Move cursor at top left position
    mov ah, 0x02
    xor bx, bx
    xor dx, dx
    int 0x10

    ; Clear screen
    mov ah, 0x06
    xor al, al
    xor bx, bx
    mov bh, 0x07
    xor cx, cx
    mov dh, 24
    mov dl, 79
    int 0x10

	;Enable A20 gate,Address 1M 
	int al,0x91H
	or  al,2
	out 0x92H,al 

	mov si,boot_msg
	call print_line

	;check it extended read is avaliable
	mov ah,0x41H
	mov bx,0x55AAH
	mov dl,0x80H
	int 13H

	;load loader program to memory

	;INT 13h AH=42h: Extended Read Sectors From Drive
	mov ah,0x42H 
	mov si,DAP 
	mov dl,0x80H 
	int 13H 

	jc read_failed 

	jmp dword 0x410H:0x0H

extension_not_supported:
	mov si,ext_read_not_support
	call print_line
	jmp $

read_failed:
	mov si,read_failed_msg
	call print_line 
	jmp $

;extend read data struct
DAP:
	.size  		db 0x10H
	.null  		db 0x0H 
	.count 		dw 3 
	.offset 	dw 0 
	.segment    dw 0x410H 
	.lba 		dd 1 
	.lba48		dd 0


boot_msg db 'Start Eospp BootLoader!',0
ext_read_not_support db 'Extendsion read not support!',0
read_failed_msg db 'read disk failed!',0


;make a real boot sector 
    times 446-($-$$) db 0