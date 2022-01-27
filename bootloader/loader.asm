org 0x10000
    jmp LoaderStart
	

%include "fat12.inc"
KernelBaseAddr		   equ 0x00
KernelOffsetAddr	   equ 0x100000
KernelTmpBaseAddr	   equ 0x00
KernelTmpOffsetAddr    equ 0x7E00
MemoryBufferAddr	   equ 0x7E00
ItemNumberPerSectors   equ 0x10; 512 / 32 = 16
ItemFileNameLen		   equ 11

[SECTION gdt]
GdtStart:	
NullDsc32 			   dd 0,0
CodeDsc32			   dd 0x0000FFFF,0x00CF9A00
DataDsc32			   dd 0x0000FFFF,0x00CF9200
GdtEnd:
Gdt32Ptr:			   dw  GdtEnd - GdtStart - 1; 
			   		   dd  GdtStart
SelectorCode32		   equ CodeDsc32 - GdtStart
SelectorData32		   equ DataDsc32 - GdtStart
KernelOffset		   dd  KernelOffsetAddr

[SECTION gdt64]
Gdt64Start:
NullDsc64 			   dq 0x0000000000000000
CodeDsc64			   dq 0x0020980000000000
DataDsc64			   dq 0x0000920000000000
Gdt64End:
Gdt64Ptr:			   dw Gdt64End - Gdt64Start - 1
					   dd Gdt64Start
SelectorCode64         equ CodeDsc64 - Gdt64Start
SelectorData64         equ DataDsc64 - Gdt64Start



[SECTION OutMsg]
EosppBinName		   db  "EOSPP   BIN",0
NoEosppBinMsg		   db  "Eospp.bin don't exist"
GetMemoryMsg		   db  "Start to get hardware memory"
FailedGetMemoryMsg	   db  "failed to get hardware memory"
SuccessGetMemoryMsg    db  "Get hardware memory successful"
StartEosppLoaderMsg    db  "Start Eospp Loader"
GetSVGAMsg			   db  "Start to get SVGA"
FailedGetSVGAMsg	   db  "failed to get SVGA"
NoSupportLongModeMsg   db  "no support long mode"
FinishLoadEosppBinMsg  db  "Load Eospp.bin successful"
FailSetSVGAModeMsg	   db  "failed to set SVGA mode"
GetSVGAMsgSuccessMsg   db  "Get SVGA info successful"
OutLine				   dw   0200H

NoSupportLongModeMsgLen 		equ 20
FailedGetSVGAMsgLen	    		equ 18
GetSVGAMsgLen		    		equ 17
EosppBinNameLen		    		equ 11
NoEosppBinMsgLen	    		equ 21
FailedGetMemoryMsgLen   		equ 29
SuccessGetMemoryMsgLen 			equ 30
GetMemoryMsgLen		    		equ 28
StartEosppLoaderMsgLen  	 	equ 18
FinishLoadEosppBinMsgLen  		equ 25
FailSetSVGAModeMsglen		    equ 23
GetSVGAMsgSuccessMsgLen		    equ 24


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
	mov ax, 	StartEosppLoaderMsgLen
	mov bx, 	StartEosppLoaderMsg
	call 		Printf
;======= open A20 =========

	push ax
	in 	 al,  	92H; 		 read one byte from 92H port
	or   al,	00000010B;   set the second bit 1
	out	 92H,	al; 		 write it to 92H port
	pop  ax
	cli ;			 	 	 close the interrupt
    db   0x66

	lgdt [Gdt32Ptr]
	mov  eax,	cr0
	or 	 eax,	1
	mov  cr0,	eax
 
	mov  ax, 	SelectorData32
	mov  fs,	ax
	mov  eax,	cr0
	and  al,	11111110B
	mov	 cr0,	eax
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
	jz	Found_EosppBin; 				  cx == 0
	dec	cx
	lodsb	
	cmp	al,		byte[es:di]
	jz	Cmp_Next
	jmp	File_Different

Cmp_Next:
	inc	di
	jmp	Cmp_File_Name

File_Different:

	and	di,		0ffe0H
	add	di,		20H
	mov	si,		EosppBinName
	jmp	Search_For_EosppBin

;========================================================
;==== printf function ===================================
;===  ax 	 : string length 
;===  bx	 : string address 
;========================================================
Printf:
    mov  cx,	ax
	mov  bp,	bx
	mov  ax,	ds
	mov  es,	ax
	mov	 ax,	1301H
	mov	 bx,	000fH
	mov	 dx,	[OutLine]	
	int	 10H
	add  word[OutLine], 0200H
	ret
Goto_Next_Sector_In_Root_Dir:
	
	add	word	[SectorNo],	1
	jmp	Search_In_Root_Dir_Begin

;========================================================
;==== read one sector function ==========================
;===  ax 	 : the start number of sector
;===  cl	 : the number of sectors that need to be read
;=== [es:bx] : pointer the address of buf
;========================================================
Read_One_Sector:
	
	push bp
	mov	 bp,			sp
	sub	 esp, 			2
	mov	 byte[bp - 2],	cl
	push bx
	mov	 bl,			[BPB_SecPerTrk]
	div	 bl
	inc	 ah; 							start sector number
	mov	 cl,			ah
	mov	 dh,			al
	shr	 al,			1;				ret >> 1,get the cylinder number
	mov	 ch,			al 
	and	 dh,			1;				cylinder number & 1,get the head number
	pop	 bx
	mov	 dl,			[BS_DrvNum]
Go_On_Reading:
	mov	ah,				2
	mov	al,				byte	[bp - 2]
	int	13h
	jc					Go_On_Reading
	add	esp,			2
	pop					bp
	ret

Get_FAT_Entry:

	push		es
	push		bx
	push		ax
	mov	ax,		00
	mov	es,		ax
	pop	ax
	mov	byte	[Odd],	0
	mov	bx,		3
	mul	bx
	mov	bx,		2
	div	bx
	cmp	dx,		0
	jz			Label_Even
	mov	byte	[Odd],	1

Label_Even:
	xor	dx,		dx
	mov	bx,		[BPB_BytesPerSec]
	div			bx
	push		dx
	mov	bx,		8000h
	add	ax,		SectorNumOfFAT1Start
	mov	cl,		2
	call		Read_One_Sector
	pop	dx
	add	bx,		dx
	mov	ax,		[es:bx]
	cmp	byte	[Odd],	1
	jnz			Label_Even_2
	shr	ax,		4

Label_Even_2:
	and	ax,		0fffh
	pop	bx
	pop	es
	ret
No_EosppBin:
	mov ax,     NoEosppBinMsgLen
	mov bx,		NoEosppBinMsg
	call 		Printf
	jmp	$

;======= found eospp.bin name in root director struct =========
Found_EosppBin:
	mov	ax,		RootDirSectors
	and	di,		0FFE0H
	add	di,		01AH
	mov	cx,		word	[es:di]; get the start cluster of eospp.bin 
	push		cx
	add	cx,		ax
	add	cx,		SectorBalance
	mov	ax,		KernelTmpBaseAddr
	mov	es,		ax
	mov	bx,		KernelTmpOffsetAddr
	mov	ax,		cx

Loading_EosppBin:
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

	mov  eax,			0x1000
	mov	 ds,			eax
	mov  dword[KernelOffset], edi
	pop	 esi 
	pop  ds 
	pop  edi 
	pop  fs 
	pop  eax 
	pop  cx 

	call 			   Get_FAT_Entry
	cmp	 ax,		   0FFFH
	jz	 			   Kernel_Loaded
	push ax
	mov  dx,		   RootDirSectors
	add  ax,		   dx
	add  ax,		   SectorBalance
	jmp  			   Loading_EosppBin

Kernel_Loaded:
	mov  ax,		  FinishLoadEosppBinMsgLen
	mov  bx,		  FinishLoadEosppBinMsg
	call			  Printf 
Kill_Motor:
	push dx
	mov  dx,		  03F2H
	mov  al,		  0
	out  dx,		  al
	pop  dx 
	
	mov  ax,		  GetMemoryMsgLen
	mov  bx,		  GetMemoryMsg
	call 			  Printf

	mov  ebx,		  0
	mov  ax,		  0x00
	mov  es,		  ax
	mov  di,		  MemoryBufferAddr 

Get_Memory:
	mov  eax,		  0x0E820
	mov  ecx,		  20
	mov  edx,		  0x534D4150
	int  15H
	jc   			  Fail_Get_Memory
	add  di,		  20
	cmp  ebx,		  0
	jne  			  Get_Memory
	jmp				  Success_Get_Memory
Fail_Get_Memory:

	mov  ax,		  FailedGetMemoryMsgLen
	mov  bx,	      FailedGetMemoryMsg
	call			  Printf
	jmp	 $

Success_Get_Memory:
	mov  ax,		  SuccessGetMemoryMsgLen
	mov  bx,		  SuccessGetMemoryMsg
	call			  Printf

Get_SVGA:
	mov  ax,		  GetSVGAMsgLen
	mov  bx,		  GetSVGAMsg
	call		  	  Printf

	mov  ax,		  0x00
	mov  es,		  ax
	mov  di,		  0x8000
	mov  ax,		  4F00H
	int  10H

	cmp  ax,		  004FH
	jz   			  Success_Get_SVGA
Fail_Get_SVGA:
	mov  ax,		  FailedGetSVGAMsgLen
	mov  bx,		  FailedGetSVGAMsg
	call 			  Printf
	jmp  $
Success_Get_SVGA:
	mov  ax,		  GetSVGAMsgSuccessMsgLen
	mov  bx,		  GetSVGAMsgSuccessMsg
	call	   		  Printf

	; 	======= set SVGA mode =================	
	mov	 ax,		  4F02h
	mov	 bx,		  4180h	
	int 			  10h
	cmp	 ax,		  004Fh
	jnz				  Set_Svga_Mode_Failed

	cli


	db   			  0x66
	lgdt 			  [Gdt32Ptr]
	
	mov	 eax,		  cr0
	or   eax,		  1
	mov  cr0,		  eax 

	jmp dword SelectorCode32:Switch_Protect

[SECTION .s32]
[BITS 32]

Switch_Protect:
	mov  ax,		  0x10
	mov  ds,		  ax
	mov  es,		  ax
	mov  fs,		  ax
	mov  ss,		  ax
	mov  esp,		  7E00H

	call 			  Support_Long_Mode
	test eax,		  eax
	jz   			  No_Support_Long_Mode


;========== temporary page table ===============
	mov	dword	[0x90000],	0x91007
	mov	dword	[0x90004],	0x00000
	mov	dword	[0x90800],	0x91007
	mov	dword	[0x90804],	0x00000

	mov	dword	[0x91000],	0x92007
	mov	dword	[0x91004],	0x00000

	mov	dword	[0x92000],	0x000083
	mov	dword	[0x92004],	0x000000

	mov	dword	[0x92008],	0x200083
	mov	dword	[0x9200c],	0x000000

	mov	dword	[0x92010],	0x400083
	mov	dword	[0x92014],	0x000000

	mov	dword	[0x92018],	0x600083
	mov	dword	[0x9201c],	0x000000

	mov	dword	[0x92020],	0x800083
	mov	dword	[0x92024],	0x000000

	mov	dword	[0x92028],	0xa00083
	mov	dword	[0x9202c],	0x000000
;===============================================

	lgdt  			  [Gdt64Ptr]
	mov   ax,		  0x10
	mov   ds,   	  ax
	mov   es,		  ax
	mov   fs,		  ax
	mov   gs,		  ax
	mov   ss,		  ax 

	mov   esp,		  7E00H
	mov   eax,        cr4
	bts   eax,		  5
	mov   cr4,		  eax

	mov   eax,		  0x90000
	mov   cr3,		  eax 

	mov   ecx,		  0C0000080H
	rdmsr
	bts   eax,		  8
	wrmsr

	mov   eax,		  cr0
	bts   eax,		  0
	bts   eax,		  31
	mov   cr0,		  eax 
	



	jmp   SelectorCode64:KernelOffsetAddr
Support_Long_Mode:
	mov   eax,	      0x80000000
	cpuid
	cmp   eax,		  0x80000001
	setnb al,
	jb    Done
	mov   eax,        0x80000001
	cpuid
	bt    edx,        29
	setc  al

Done:
	movzx  eax,       al
	ret 

No_Support_Long_Mode:
	mov  ax,		  NoSupportLongModeMsgLen
	mov  bx,	      NoSupportLongModeMsg
	call			  Printf
	jmp  $

Set_Svga_Mode_Failed:
	mov  ax,		  FailSetSVGAModeMsglen
	mov  bx,		  FailSetSVGAModeMsg
	call		      Printf
	jmp  $

