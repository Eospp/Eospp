RootDirSectors			equ	14
RootDirStartSectorNum   equ 19
SectorNumOfFAT1Start	equ	1
SectorBalance			equ	17	
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