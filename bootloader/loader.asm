org 10000H
    jmp LoaderStart
StartEosppLoaderMsgLen equ 18
StartEosppLoaderMsg    db "Start Eospp Loader"

LoaderStart:
    mov	ax,	cs
	mov	ds,	ax
	mov	es,	ax
	mov	ax,	0x00
	mov	ss,	ax
	mov	sp,	0x7c00

;=======	display on screen : Start Loader......
Display:
	mov	ax,	1301h
	mov	bx,	000fh
	mov	dx,	0200h		;row 2
	mov	cx,	StartEosppLoaderMsgLen
	push	ax
	mov	ax,	ds
	mov	es,	ax
	pop	ax
	mov	bp,	StartEosppLoaderMsg
	int	10h

	jmp	$
 
