			.data
msg1:       .asciz "*****Input ID*****\n"
msg2:       .asciz "** Please Enter Member 1 ID : **\n"
msg3:       .asciz "** Please Enter Member 2 ID : **\n"
msg4:       .asciz "** Please Enter Member 3 ID : **\n"
msg5:       .asciz "** Please Enter Command **\n"
msg6:       .asciz "*****Print Team Member ID and ID Summation*****\n"
msg7:       .asciz "ID Summation = %d\n"
msg8:		.asciz "*****End Print*****\n\n"
str1:		.asciz "%d"
str2:		.asciz	"%s" @enter command
str3:		.asciz "\n"
enterID:	.asciz	"%d\n"
command:	.asciz	"\0"
id1:		.word	0
id2:		.word	0
id3:		.word	0
idSum:		.word	0
			.text
			.global msg7  @ "ID Summation = %d\n"
			.global idSum
            .global id
			.global	id1
			.global id2
			.global id3
            .type  id, %function



id:
	stmfd	sp!,{lr}
	ldr		r0, =msg1
	bl		printf	@"*****Input ID*****\n"

	mov 	r2,r1	@r2 = r1 back up r1
	subs 	r1, pc, lr	@r1 = pc - lr and set cpsr
	mov		r1,r2
@id1-------------------------------------------------------------

	ldr		r0, =msg2
	bl		printf	@"** Please Enter Member 1 ID : **\n"
	ldr		r0, =str1	@r0 = %d
	ldr		r1, =id1
	bl scanf	@ call scanf("%d",&id1)

@id2-------------------------------------------------------------
	ldr		r0, =msg3
	bl		printf	@"** Please Enter Member 2 ID : **\n"
	ldr		r0, =str1	@%d
	ldr		r1, =id2
	bl scanf	@ call scanf("%d",&id2)

@id3-------------------------------------------------------------
	ldr		r0, =msg4
	bl		printf	@"** Please Enter Member 3 ID : **\n"
	ldr		r0, =str1	@%d
	ldr		r1, =id3
	bl scanf	@ call scanf("%d",&id3)
@enter command---------------------------------------------------
	ldr		r0, =msg5
	bl		printf	@"** Please Enter Command **\n"
loop:
	ldr		r0, =str2	@%s
	ldr		r1, =command	@load address of char
	bl		scanf
	ldr     r1, =command
	ldrb	r1,[r1]	@load char variable**
	cmp		r1,#'p'	@**
	bne		loop	@if(r1!='p') loop
@Print All-------------------------------------------------------
	ldr		r0,=msg6
	bl		printf	@"*****Print Team Member ID and ID Summation*****\n"
@Print id1-------------------------------------------------------
	ldr		r0,=enterID	@%d\n
	ldr		r1,=id1	@load id1 address
	mov		r5,r1
	mov		r6,#0
	ldr		r1,[r5,r6]	@load id1 variable r1 = r5+r6**
	bl		printf	@Print id1
@Print id2-------------------------------------------------------
	ldr		r0,=enterID	@%d\n
	ldr		r1,=id2	@load id1 address
	mov 	r5,r1	@r5= r1
	sub		r5,r5,#4	@r5 = r5-4
	ldr 	r1,[r5,#4]	@load id1 variable**
	bl		printf	@Print id1
@Print id3-------------------------------------------------------
	ldr		r0,=enterID	@%d\n
	ldr		r1,=id3	@load id1 address
	mov 	r5,r1	@r5 = r1
	sub		r5,r5,#1	@r5 = r5-1
	ldr 	r1,[r5,#1]!	@load id1 variable r1 = r5+1, r5 = r5+1**
	bl		printf	@Print id1

	ldr		r0,=str3	@\n
	bl		printf
@calculate IDsum-----------------------------------------------------
	ldr		r1,=id1	@load id1 address
	ldr		r1,[r1]	@load id1 variable

	ldr		r2,=id2	@load id2 address
	ldr		r2,[r2]	@load id2 variable

	cmp		r2,#0
	addges	r1,r1,r2	@if(ge)r1 = r1+r2	set cpsr**

	ldr		r3,=id3	@load id3 address
	ldr		r3,[r3]	@load id3 variable

	addgts	r1,r1,r3	@if(gt)	r1 = (r1+r2)+r3 conditional excution**

	ldrne		r4,=idSum	@if(ne)load idSum address to r4**
	strne		r1,[r4]	@if(ne)store r1's value to r4's value
	ldr		    r0,=msg7	@"ID Summation = %d\n"
	bl		    printf
@endprint-----------------------------------------------------------
	ldr		r0,=msg8
	bl 		printf @"*****End Print*****\n\n"
	mov 	r0,#0	@load return value
	ldmfd 	sp!,{lr}	@pop link register from stack
	mov	pc,lr	@return from main


