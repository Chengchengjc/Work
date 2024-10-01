	.arch armv6
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu vfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"main.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Function1: Name\000"
	.align	2
.LC1:
	.ascii	"Function2: ID\000"
	.align	2
.LC2:
	.ascii	"Main Function:\000"
	.align	2
.LC3:
	.ascii	"*****Print All*****\000"
	.align	2
.LC4:
	.ascii	"%s\000"
	.align	2
.LC5:
	.ascii	"%d, %s\000"
	.align	2
.LC6:
	.ascii	"ID Summation = %d\012\000"
	.align	2
.LC7:
	.ascii	"*****End Print*****\000"
	.align	2
.LC8:
	.ascii	"\012***** Please enter p to draw Julia Set animatio"
	.ascii	"n *****\000"
	.align	2
.LC9:
	.ascii	"clear\000"
	.align	2
.LC10:
	.ascii	"/dev/fb0\000"
	.align	2
.LC11:
	.ascii	"Frame Buffer Device Open Error!!\000"
	.align	2
.LC12:
	.ascii	".*.*.*.<:: Happy New Year ::>.*.*.*.\000"
	.align	2
.LC13:
	.ascii	"%d     %s\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 614424
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #614400
	sub	sp, sp, #24
	ldr	r3, .L9
	str	r3, [fp, #-12]
	ldr	r3, .L9
	str	r3, [fp, #-16]
	ldr	r3, .L9+4
	str	r3, [fp, #-20]
	mvn	r3, #4
	str	r3, [fp, #-24]
	ldr	r0, .L9+8
	bl	puts
	bl	name
	ldr	r0, .L9+12
	bl	puts
	bl	id
	ldr	r0, .L9+16
	bl	puts
	ldr	r0, .L9+20
	bl	puts
	ldr	r3, .L9+24
	mov	r0, r3
	ldr	r1, .L9+28
	bl	printf
	ldr	r2, .L9+32
	ldr	r3, .L9+36
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	ldr	r2, .L9+40
	bl	printf
	ldr	r2, .L9+32
	ldr	r3, .L9+44
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	ldr	r2, .L9+48
	bl	printf
	ldr	r2, .L9+32
	ldr	r3, .L9+52
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	ldr	r2, .L9+56
	bl	printf
	ldr	r2, .L9+60
	ldr	r3, .L9+64
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	bl	printf
	ldr	r0, .L9+68
	bl	puts
	ldr	r0, .L9+72
	bl	puts
	mov	r0, r0	@ nop
.L2:
	bl	getchar
	mov	r3, r0
	cmp	r3, #112
	bne	.L2
	ldr	r0, .L9+76
	bl	system
	ldr	r0, .L9+80
	ldr	r1, .L9+84
	bl	open
	str	r0, [fp, #-28]
	ldr	r3, [fp, #-28]
	cmp	r3, #0
	bge	.L3
	ldr	r0, .L9+88
	bl	puts
	b	.L8
.L3:
	mov	r3, #400
	str	r3, [fp, #-8]
	b	.L5
.L6:
	sub	r3, fp, #614400
	sub	r3, r3, #4
	sub	r3, r3, #24
	ldr	r0, [fp, #-8]
	mov	r1, r3
	bl	drawJuliaSet
	sub	r3, fp, #614400
	sub	r3, r3, #4
	sub	r3, r3, #24
	ldr	r0, [fp, #-28]
	mov	r1, r3
	mov	r2, #614400
	bl	write
	ldr	r0, [fp, #-28]
	mov	r1, #0
	mov	r2, #0
	bl	lseek
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-24]
	add	r3, r2, r3
	str	r3, [fp, #-8]
.L5:
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-20]
	cmp	r2, r3
	bge	.L6
	ldr	r0, .L9+92
	bl	puts
	ldr	r0, .L9+28
	bl	puts
	ldr	r2, .L9+96
	ldr	r3, .L9+36
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	ldr	r2, .L9+40
	bl	printf
	ldr	r2, .L9+96
	ldr	r3, .L9+44
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	ldr	r2, .L9+48
	bl	printf
	ldr	r2, .L9+96
	ldr	r3, .L9+52
	ldr	r3, [r3, #0]
	mov	r0, r2
	mov	r1, r3
	ldr	r2, .L9+56
	bl	printf
	ldr	r0, [fp, #-28]
	bl	close
.L8:
	mov	r0, r0	@ nop
.L7:
	bl	getchar
	mov	r3, r0
	cmp	r3, #112
	bne	.L7
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L10:
	.align	2
.L9:
	.word	-700
	.word	270
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	teamNo
	.word	.LC5
	.word	id1
	.word	name1
	.word	id2
	.word	name2
	.word	id3
	.word	name3
	.word	.LC6
	.word	idSum
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	1052674
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.size	main, .-main
	.ident	"GCC: (Debian 4.6.3-8+rpi1) 4.6.3"
	.section	.note.GNU-stack,"",%progbits
