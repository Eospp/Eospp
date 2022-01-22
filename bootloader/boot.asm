org 0x7c00 ; the start address of boot
StackBase 				equ 0x7c00 
RootDirSectors			equ	14
RootDirStartSectorNum   equ 19
SectorNumOfFAT1Start	equ	1
ItemFileNameLen			equ 11
ItemNumberPerSectors    equ 0x10; 512 / 32 = 16
BaseOfLoader			equ 0x1000
OffsetOfLoader  		equ 0x00
SectorBalance			equ	17	
StartEosppBootMsgLen    equ 16 
NoEosppLoaderMsgLen 	equ 24

	jmp	short Start
	nop
StartEosppBootMsg 		db "Start Eospp boot" 
NoEosppLoaderMsg   		db "Eospp loader don't exist"
LoaderFileName			db "LOADER  BIN",1
BS_OEMName				db	'MINEboot'
BPB_BytesPerSec			dw	512		;the number of bytes per sectors
BPB_SecPerClus			db	1		;the number of sectors per cluster
BPB_RsvdSecCnt			dw	1		;the number of reserved sectors
BPB_NumFATs				db	2		;the number of fat table,it is set to 2 so that a table can be backed up
BPB_RootEntCnt			dw	224 	;the number of items that root can hold
BPB_TotSec16			dw	2880 	;the total numbers of sectors
BPB_Media				db	0xf0
BPB_FATSz16				dw	9		;the number of sectors that fat table need
BPB_SecPerTrk			dw	18		
BPB_NumHeads			dw	2
BPB_HiddSec				dd	0
BPB_TotSec32			dd	0
BS_DrvNum				db	0
BS_Reserved1			db	0	
BS_BootSig				db	0x29
BS_VolID				dd	0
BS_VolLab				db	'boot loader'
BS_FileSysType			db	'FAT12   '
SectorNo				dw	0
Odd						db	0
RootDirSizeForLoop		dw	RootDirSectors
Start: 
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,StackBase
;======= clear screen ===============
	mov	ax,	0600H
	mov	bx,	0700H
	mov	cx,	0
	mov	dx,	0184fH
	int	10h
;======	set focus ============
	mov	ax,	0200H
	mov	bx,	0000H
	mov	dx,	0000H
	int	10H
;======= display on screen : Start Eospp boot ===================
	mov	ax,	1301H; AH = 13H AL = 01H
	mov	bx,	000fH
	mov	dx,	0000H
	mov	cx,	StartEosppBootMsgLen
	push	ax
	mov	ax,	ds
	mov	es,	ax
	pop	ax
	mov	bp,	StartEosppBootMsg; ebp save the address of StartEosppotMsg
	int	10h
;======= reset floppy =============================
	xor	ah,	ah
	xor	dl,	dl
	int	13h
;======= search loader.bin ========================
	mov	word	[SectorNo],	RootDirStartSectorNum

Search_In_Root_Dir_Begin:

	cmp	word	[RootDirSizeForLoop],	0
	jz	No_LoaderBin
	dec	word	[RootDirSizeForLoop]; root dir have ${RootDirSectors} sectors,so it will loop ${RootDirSectors} times to read
	mov	ax,	00H
	mov	es,	ax
	mov	bx,	8000H;					  [es:bx] pointer the address of buf
	mov	ax,	[SectorNo];				  the start number of sector
	mov	cl,	1; 						  the number of sectors that need to be read
	call	Read_One_Sector
	mov	si,	LoaderFileName
	mov	di,	8000H
	cld
	mov	dx,	ItemNumberPerSectors
	
Search_For_LoaderBin:

	cmp	dx,	0
	jz	Goto_Next_Sector_In_Root_Dir ; dx == 0
	dec	dx
	mov	cx, ItemFileNameLen	

Cmp_File_Name:

	cmp	cx,	0
	jz	Loader_Found; cx == 0
	dec	cx
	lodsb	
	cmp	al,	byte	[es:di]
	jz	Cmp_Next
	jmp	File_Different

Cmp_Next:
	
	inc	di
	jmp	Cmp_File_Name

File_Different:

	and	di,	0ffe0h
	add	di,	20h
	mov	si,	LoaderFileName
	jmp	Search_For_LoaderBin

No_LoaderBin:

	mov	ax,	1301h
	mov	bx,	008ch
	mov	dx,	0100h
	mov	cx,	NoEosppLoaderMsgLen
	push	ax
	mov	ax,	ds
	mov	es,	ax
	pop	ax
	mov	bp,	NoEosppLoaderMsg
	int	10h
	jmp	$
;======= found loader.bin name in root director struct =========

Loader_Found:

	mov	ax,	RootDirSectors
	and	di,	0ffe0h
	add	di,	01ah
	mov	cx,	word	[es:di]
	push	cx
	add	cx,	ax
	add	cx,	SectorBalance
	mov	ax,	BaseOfLoader
	mov	es,	ax
	mov	bx,	OffsetOfLoader
	mov	ax,	cx

Go_On_Loading_File:
	push	ax
	push	bx
	mov	ah,	0eh
	mov	al,	'.'
	mov	bl,	0fh
	int	10h
	pop	bx
	pop	ax

	mov	cl,	1
	call	Read_One_Sector
	pop	ax
	call	Get_FAT_Entry
	cmp	ax,	0fffh
	jz	File_Loaded
	push	ax
	mov	dx,	RootDirSectors
	add	ax,	dx
	add	ax,	SectorBalance
	add	bx,	[BPB_BytesPerSec]
	jmp	Go_On_Loading_File

File_Loaded:
	
	jmp	BaseOfLoader:OffsetOfLoader

Goto_Next_Sector_In_Root_Dir:
	
	add	word	[SectorNo],	1
	jmp	Search_In_Root_Dir_Begin
Read_One_Sector:
	
	push bp
	mov	 bp,	sp
	sub	 esp, 2
	mov	 byte[bp - 2],cl
	push bx
	mov	 bl,[BPB_SecPerTrk]
	div	 bl
	inc	 ah; 		start sector number
	mov	 cl,ah
	mov	 dh,al
	shr	 al,1;		ret >> 1,get the cylinder number
	mov	 ch,al 
	and	 dh,1;		cylinder number & 1,get the head number
	pop	 bx
	mov	 dl,[BS_DrvNum]
Go_On_Reading:
	mov	ah,	2
	mov	al,	byte	[bp - 2]
	int	13h
	jc	Go_On_Reading
	add	esp,	2
	pop	bp
	ret
Get_FAT_Entry:

	push	es
	push	bx
	push	ax
	mov	ax,	00
	mov	es,	ax
	pop	ax
	mov	byte	[Odd],	0
	mov	bx,	3
	mul	bx
	mov	bx,	2
	div	bx
	cmp	dx,	0
	jz	Label_Even
	mov	byte	[Odd],	1

Label_Even:

	xor	dx,	dx
	mov	bx,	[BPB_BytesPerSec]
	div	bx
	push	dx
	mov	bx,	8000h
	add	ax,	SectorNumOfFAT1Start
	mov	cl,	2
	call	Read_One_Sector
	
	pop	dx
	add	bx,	dx
	mov	ax,	[es:bx]
	cmp	byte	[Odd],	1
	jnz	Label_Even_2
	shr	ax,	4

Label_Even_2:
	and	ax,	0fffh
	pop	bx
	pop	es
	ret

;=======	fill zero until whole sector
	times	510 - ($ - $$)	db	0
	dw	0xaa55
