org 10000H
    jmp short LoaderStart
	nop

%include "fat12.inc"
StartEosppLoaderMsgLen equ 18
KernelBaseAddr		   equ 0x00
KernelOffsetAddr	   equ 0x100000
KernelTmpBaseAddr	   equ 0x00
KernelTmpOffsetAddr    equ 0x7E00
ItemNumberPerSectors   equ 0x10; 512 / 32 = 16
ItemFileNameLen		   equ 11
StartEosppLoaderMsg    db "Start Eospp Loader"
GdtStart:	
NullDsc 			   dd 0,0
CodeDsc32			   dd 0x0000FFFF,0x00CF9A00
DataDsc32			   dd 0x0000FFFF,0x00CF9200
GdtEnd:
GdtPtr:				   dw  GdtEnd - GdtStart - 1
			   		   dw  GdtStart
SelectorCode32		   equ CodeDsc32 - GdtStart
SelectorData32		   equ DataDsc32 - GdtStart

EosppBinName		   db "EOSPP   BIN",0
EosppBinNameLen		   equ 11

NoEosppBinMsg		   db  "Eospp.bin don't exist"
NoEosppBinMsgLen	   equ 21

KernelOffset		   dd KernelOffsetAddr

[SECTION .s16]
[BITS 16]
LoaderStart:
    mov	 ax,	cs
	mov	 ds,	ax
	mov	 es,	ax
	mov	 ax,	0x00
	mov	 ss,	ax
	mov  sp,	0x7c00

;======= display on screen : Start Eospp Loader ========
Display:
	mov	 ax,	1301h
	mov	 bx,	000fh
	mov	 dx,	0200h	
	mov	 cx,	StartEosppLoaderMsgLen
	push ax
	mov	 ax,	ds
	mov	 es,	ax
	pop	 ax
	mov  bp,	StartEosppLoaderMsg
	int	 10h

;======= open A20 =========

	push ax
	in 	 al,  	92H; 		 read one byte from 92H port
	or   al,	00000010B;   set the second bit 1
	out	 92H,	al; 		 write it to 92H port
	pop  ax
	cli ;			 	 	 close the interrupt
    db   0x66

	lgdt [GdtPtr]
	mov  eax,	cr0
	or 	 eax,	1
	mov  cr0,	eax
 
	mov  ax, 	SelectorData32
	mov  fs,	ax
	mov  eqx,	cr0
	and  al,	11111110B
	mov	 cr0,	eqx
	sti ;					open the interrupt


;======= reset floppy =========

	xor ah,		ah
	xor dl,		dl
	int 13H

;======= search eospp.bin ======

	mov	word	[SectorNo],	RootDirStartSectorNum

Search_In_Root_Dir_Begin:

	cmp	word	[RootDirSizeForLoop],	0
	jz	No_EosppBin
	dec	word	[RootDirSizeForLoop]; 	  root dir have ${RootDirSectors} sectors,so it will loop ${RootDirSectors} times to read
	mov	ax,		00H
	mov	es,		ax
	mov	bx,		8000H;					  [es:bx] pointer the address of buf
	mov	ax,		[SectorNo];				  the start number of sector
	mov	cl,		1; 						  the number of sectors that need to be read
	call		Read_One_Sector
	mov	si,		EosppBinName
	mov	di,		8000H
	cld
	mov	dx,		ItemNumberPerSectors
	
Search_For_EosppBin:

	cmp	dx,		0
	jz	Goto_Next_Sector_In_Root_Dir ; 	  dx == 0
	dec	dx
	mov	cx, 	ItemFileNameLen	

Cmp_File_Name:

	cmp	cx,		0
	jz	EosppBin_Found; 				  cx == 0
	dec	cx
	lodsb	
	cmp	al,		byte[es:di]
	jz	Cmp_Next
	jmp	File_Different

Cmp_Next:
	
	inc	di
	jmp	Cmp_File_Name

File_Different:

	and	di,		0ffe0h
	add	di,		20h
	mov	si,		EosppBinName
	jmp	Search_For_EosppBin

No_EosppBin:

	mov	ax,		1301h
	mov	bx,		008ch
	mov	dx,		0100h
	mov	cx,		NoEosppBinMsgLen
	push		ax
	mov	ax,		ds
	mov	es,		ax
	pop	ax
	mov	bp,		NoEosppBinMsg
	int	10h
	jmp	$

;======= found eospp.bin name in root director struct =========
EosppBin_Found:

	mov	ax,		RootDirSectors
	and	di,		0ffe0h
	add	di,		01ah
	mov	cx,		word	[es:di]
	push		cx
	add	cx,		ax
	add	cx,		SectorBalance
	mov	ax,		KernelTmpBaseAddr
	mov	es,		ax
	mov	bx,		KernelTmpOffsetAddr
	mov	ax,		cx

Loading_EosppBin:
	push		ax
	push		bx
	mov	ah,		0eh
	mov	al,		'.'
	mov	bl,		0fh
	int	10h
	pop	bx
	pop	ax

	mov	 cl,	1
	call Read_One_Sector
	pop	 ax
	
	push cx 
	push eax 
	push fs 
	push edi 
	push ds
	push esi 

	mov  cx,	200H
	mov  ax,	KernelBaseAddr
	mov  fs,	ax
	mov  edi,	dword[KernelOffset]

	mov  ax,	KernelTmpBaseAddr
	mov	 ds,	ax
	mov  esi,	KernelTmpOffsetAddr	


Mov_Kernel:
	mov  al,			byte[ds:esi]
	mov  byte[fs:edi],  al 
	inc	 esi 
	inc  edi 
	loop Mov_Kernel

	mov  eax,			0x1000H
	mov	 ds,			eax
	mov  dword[KernelOffset], edi
	pop	 esi 
	pop  ds 
	pop  edi 
	pop  fs 
	pop  eax 
	pop  cx 

	call Get_FAT_Entry
	cmp	 ax,		   0FFFH
	jz	 Kernel_Loaded
	push ax
	mov  dx,		   RootDirSectors
	add  ax,		   dx
	add  ax,		   SectorBalance
	jmp  Loading_EosppBin

Kernel_Loaded:
	mov  ax,  		  0B800H
	mov  gs,		  ax
	mov  ah,		  0FH
	mov  al,		  'G'
	mov  [gs:((80 * 0 + 39) * 2)], ax

Kill_Motor:
	push dx
	mov  dx,		  03F2H
	mov  al,		  0
	out  dx,		  al
	pop  dx 