[BITS 16]

jmp start

%include "io.asm"

BUFFER_SEGMENT equ 0x5000H
BUFFET_BASE    equ 0x4500H

start:
	mov ax, 0410H 
	mov ds, ax 

	mov ax,BUFFER_SEGMENT
	mov es,ax 

	mov si,load_kernel_msg
	call print_line

	;1.get pratition table from MBR 

	mov byte [DAP.count],1
	
	mov word [DAP.segment],BUFFER_SEGMENT
	mov dword[DAP.lba],0
	call extend_read

	;read 2 bytes,because only one partition now
	mov ax,[es:(BUFFER_BASE + 446 + 8)]
	mov [pratition_start],ax
	
	;2.get FAT information from VBR of partition
	mov byte [DAP.count],1
	mov word [DAP,offset],BUFFET_BASE
	mov word [DAP.segment],BUFFER_SEGMENT
	mov dword[DAP.lba],ax
	call extend_read

	mov ah,[es:(BUFFER_BASE + 13)]
	mov [sectors_per_cluster],ah 

	mov ax,[es:(BUFFER_BASE + 14)]
	mov [reserved_sectors],ax 

	mov ah,[es:(BUFFER_BASE + 16)]
	mov [fat_nubmer],ah 

	mov ax,[es:(BUFFER_BASE + 38)]
	test ax,ax 
	jne sectors_per_fat_too_high

	mov ax,[es:(BUFFER_BASE + 36)]
	mov [sectors_per_fat],ax 

	mov ax,[es:(BUFFER_BASE + 44)]
	mov [root_dir_start],ax

	;fat_begin_sector = partition_start + reserved_sectors
	mov ax,[partition_start]
	mov bx,[reserved_sectors]
	add ax,bx
	mov [fat_begin_sector],ax

	;cluster_begin_sector = fat_begin_sector + fat_nubmer * sectors_per_fat
	mov ax,[sectros_per_fat]
	mov bx,[fat_nubmer]
	mul bx
	mov bx,[fat_begin_sector]
	add ax,bx
	mov [cluster_begin_sector],ax
	
	;entries per cluster = (512 / 32) * sectors_per_cluster
	movzx ax,byte [sectors_per_cluster]
	shl ax,4 
	mov [entries_per_cluster],ax

	;3.find the kernel.bin file from cluster
	mov ah,[sectors_per_cluster]
	mov byte[DAP.count],ah 
	mov word[DAP.offset],BUFFER_BASE
	mov word[DAP.segment],BUFFER_SEGMENT

	mov ax,[root_dir_start]
	sub ax,2
	movzx bx,byte [sectors_per_cluster]
	mul bx,
	mov bx,[cluster_begin_sector]
	add ax,bx 

	mov word[DAP.lba],ax 
	call extend_read 

	




sectors_per_fat_too_high:
	mov si,sectors_per_fat_too_high 
	call print_line
	jmp $



extend_read:
	mov ah,0x42H
	mov si,DAP
	mov dl,0x80H 
	int 13H
	
	jc read_failed 
	ret 
	



read_failed:
	mov si read_failed_msg
	call print_line
	jmp $







DAP:
	.size 		db 0x10H
	.null		db 0x0
	.count 		dw 0
	.offset 	dw 0
	.segment  	dw 0x0H 
	.lba 		dd 0 
	.lba48		dd 0



load_kernel_msg				db 'load eospp kernel!',0
read_failed_msg     		db 'read disk failed!',0
disk_big_msg				db 'The disk is too big!',0


pratition_start 			dw 0
sectors_per_cluster 		db 0
reserved_sectors			dw 0
fat_nubmer  				db 0
fat_begin_sector 		 	dw 0
sectors_per_fat				dw 0
root_dir_start 				dw 0
cluster_begin_sector		dw 0
entries_per_cluster   		dw 0