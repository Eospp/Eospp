[BITS 16]

jmp start

%include "io.asm"

BUFFER_SEGMENT equ 0x5000
BUFFER_BASE    equ 0x4500
KERNEL_BASE	   equ 0x600

start:
	mov ax, 0x410 
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
	mov [partition_start],ax
	
	;2.get FAT information from VBR of partition
	mov byte [DAP.count],1
	mov word [DAP.offset],BUFFER_BASE
	mov word [DAP.segment],BUFFER_SEGMENT
	mov word[DAP.lba],ax
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
	mov ax,[sectors_per_fat]
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

	mov si,BUFFER_BASE
	xor cx,cx	

next:
	mov ah,[es:si]
	test ah,ah 
	je end_of_directory

	mov ax,[es:si]
	cmp ax,0x4E49
	jne continue

	mov ax,[es:(si + 2)]
	cmp ax,0x5449
	jne continue 
	
	mov ax,[es:(si + 4)]
	cmp ax,0x2020
	jne continue 
	
	mov ax,[es:(si + 6)]
	cmp ax,0x2020
	jne continue 

	mov ax,[es:(si + 8)]
	cmp ax,0x4942
	jne continue

	mov ah,[es:(si + 10)]
	cmp ah,0x4E
	jne continue

	mov ax,[es:(si + 20)]
	mov [kernel_start_cluster_high],ax
	mov ax,[es:(si + 26)]
	mov [kernel_start_cluster_low],ax
	jmp found



continue:
	add si,32
	inc cx 

	mov bx,[entries_per_cluster]
	cmp cx,bx 
	jne next

end_of_cluster:
	mov si,multi_cluster_directory_msg
	call print_line
	jmp $

end_of_directory:
	mov si,kernel_not_fonund_msg
	call print_line
	jmp $

found:
	mov si,kernel_fonund_msg
	call print_line
	;load kernel to memory	
	mov ax,[kernel_start_cluster_high]
	test ax,ax
	jne cluster_too_high

	mov ax,[kernel_start_cluster_low]
	mov word[cluster_current],ax
	mov word[segment_current],KERNEL_BASE

next_cluster:
	
	movzx ax,[sectors_per_cluster]
	mov word[DAP.count],ax
	mov word[DAP.offset],0
	mov ax,[segment_current]
	mov [DAP.segment],ax
	;compute start sector to read
	mov ax,[cluster_current]
	sub ax,2
	movzx bx,byte[sectors_per_cluster]
	mul bx
	mov bx,[cluster_begin_sector]
	add ax,bx
	mov word[DAP.lba],ax

	call extend_read
	;compute the sector of FAT to read
	;FAT sector = cluster_current / (512 / 4)
	mov ax,[cluster_current]
	shl ax,2
	shr ax,9
	mov bx,[fat_begin_sector]
	add ax,bx

	;read FAT sector
	mov word[DAP.count],1
	mov word[DAP.offset],BUFFER_BASE
	mov word[DAP.segment],BUFFER_SEGMENT
	mov word[DAP.lba],ax
	call extend_read

	;get next cluster
	mov si,[cluster_current]
	and si,128 - 1
	shl si,2

	mov ax,[es:(BUFFER_BASE + si)]
	mov bx,[es:(BUFFER_BASE + si + 2)] 

	cmp bx,0xFFF
	jl ok

	cmp ax,0xFFF7
	je corrupted

	cmp ax,0xFFF8
	jge fully_loaded

ok:
	test bx,bx 
	jne cluster_too_high
	mov [cluster_current],ax
	;segment += sectors * (512 / 16)
	movzx ax,byte [sectors_per_cluster]
	shl ax,5
	mov bx,[segment_current]
	add ax,bx
	mov [segment_current],ax
	jmp next_cluster

fully_loaded:
	mov si,kernel_loaded_msg
	call print_line
	call KERNEL_BASE:0x0
	jmp $

corrupted:
	mov si,corrupted_disk_msg
	call print_line
	jmp $

sectors_per_fat_too_high:
	mov si,sectors_per_fat_too_high 
	call print_line
	jmp $

cluster_too_high:
    mov si,cluster_too_high_msg
	call print_line
	jmp $

extend_read:
	mov ah,0x42
	mov si,DAP
	mov dl,0x80 
	int 13H

	jc read_failed 
	ret 


read_failed:
	mov si,read_failed_msg
	call print_line
	jmp $

DAP:
	.size 		db 0x10
	.null		db 0x0
	.count 		dw 0
	.offset 	dw 0
	.segment  	dw 0x0 
	.lba 		dd 0 
	.lba48		dd 0



load_kernel_msg				db 'load eospp kernel!',0
read_failed_msg     		db 'read disk failed!',0
disk_big_msg				db 'The disk is too big!',0
kernel_not_fonund_msg		db 'eospp kernel not fonund!',0
kernel_fonund_msg			db 'eospp kernel fonund!',0
kernel_loaded_msg			db 'eospp kernel loaded fully',0
multi_cluster_directory_msg db 'directory not support store multi cluster now!',0
cluster_too_high_msg		db 'cluster too high!',0
corrupted_disk_msg			db 'The disk seems to be corrupted',0

cluster_current 			dw 0
segment_current				dw 0
partition_start 			dw 0
sectors_per_cluster 		db 0
reserved_sectors			dw 0
fat_nubmer  				db 0
fat_begin_sector 		 	dw 0
sectors_per_fat				dw 0
root_dir_start 				dw 0
cluster_begin_sector		dw 0
entries_per_cluster   		dw 0
kernel_start_cluster_high   dw 0
kernel_start_cluster_low    dw 0

	times 512 * 3 - ($-$$)  db 0