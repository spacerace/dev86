
org $7c00

include sysboot.s

org dos_sysid
   .ascii "PANIC"	! System ID

org codestart
  xor	ax,ax
  mov	ds,ax
  mov	ss,ax
  mov	sp,ax
  jmpi	code,#0

no_os:
  .asciz	"PANIC! NO OS Found!\r\n"

code:		! SI = pointer to error message
  mov	si,#no_os
nextc:
  lodsb
  cmp	al,#0
  jz	eos
  mov	bx,#7
  mov	ah,#$E		! Can't use $13 cause that's AT+ only!
  int	$10
  jmp	nextc
eos:			! Wait for a key then reboot
  xor	ax,ax
  int	$16
  jmpi	$0,$FFFF	! Wam! Try or die!
