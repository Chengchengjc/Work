@ r4 = zx
@ r5 = zy
@ r6 = tmp
@ r7 = i, color
@ r8 = x
@ r9 = y
@ r10 = cY
@ r11 = frame
@ ==========================================
.data

.text
	.const:
		.word	1500
		.word 	4000000
		.word 	0xffff

    .global drawJuliaSet
    .type  drawJuliaSet, %function

    drawJuliaSet:
        stmfd sp!, {r4-r11,lr}
        mov r10,r0 @ r0=cY,r10 = cY
		mov r11,r1	@r1 = frame,r11 = frame
        mov r8, #0	@initialize x = 0
    loopOne:
        cmp r8, #640 @for (x = 0; x < 640; x++)
        bge DoneOne @ if r8>=640 do DoneOne
        mov r9, #0 @y = 0
    loopTwo:
        cmp r9, #480 @for (y = 0; y < 480; y++)
        bge DoneTwo @ color = ((i&0xff)<<8) | (i&0xff)....

		@--zx = 1500 * (x - 320) / 320;-----------------------

        ldr r0, .const	@r0 = 1500
		sub r1, r8,#320	@r1 = r8 -320 ( r8 = x)
		mul r0,r0,r1	@ r0 = r0 * r1 ( r0 = 1500 * (x - 320) )
		mov r1,#320
		bl __aeabi_idiv @r0 = r0/r1
		mov r4,r0	@zx = 1500 * (x - 320) / 320

		@--zy = 1000 * (y - 240) / 240;-----------------------
		sub r1, r9,#240	@r1 = r9 -240 ( r9 = y)
		mov r2,#1000 @ r2 = 1000
		mul r0,r1,r2	@ r0 = r1 * 1000 ( r0 = 1000 * (y - 240) )
		mov r1,#240
        bl __aeabi_idiv @r0 = r0/r1	= r0 / 240 (r0 = 1000 * (y - 240) / 240)
		mov r5,r0	@zy = 1000 * (y - 240) / 240

		mov r7, #255 @i = 255

	WhileCond:	@while(zx * zx + zy * zy < 4000000 && i > 0)
		mul r0,r4,r4	@r0 = r4*r4 ( r0 = zx * zx )
		mul r1,r5,r5	@r1 = r5*r5 ( r1 = zy * zy )
		add r0,r0,r1	@r0 = r0*r1 ( r0 = zx * zx +zy * zy)
		ldr r2, .const+4
		cmp r0, r2	@zx * zx + zy * zy < 4000000
		bge doneWhile @ end while
		cmp r7,#0	@i > 0
		ble doneWhile @ end while
		@inside while---------------------------
		mul r0,r4,r4	@ r0 =zx * zx
		mul r1,r5,r5	@ r1 = zy*zy
		sub r0,r0,r1	@r0 = zx*zx - zy*zy
		mov r1,#1000
		bl __aeabi_idiv @r0 = r0/r1
		sub r6,r0,#700	@r6 = (zx*zx-zy*zy/1000)-700	(r6 = tmp)
		mul r0,r4,r5    @r0 = r4*r5	(r0= zx * zy)
        mov r1,#500 @r0 = r0/500	(r0= (zx * zy)/500)
        bl __aeabi_idiv @r0/r1
		add r0,r0,r10 @r0 = r0+r10 (r10 = cY, r0 =(zx * zy)/500)
		mov r5,r0	@r5=r0 (r5 = zy)
		mov r4, r6 @ zx = tmp (r4 = zx)
		sub r7,r7,#1 @i-- (r7 = i)

		b WhileCond


	doneWhile:
		@color = ((i&0xff)<<8) | (i&0xff);
		@color = (~color)&0xffff;
		@frame[y][x] = color;
	    and   r7, r7, #0xff      @ r7 = i, i = i & 0xff

		mov   r0, #8

		orr   r7, r7, r7, lsl r0 @ r7 = (i & 0xff) | ((i & 0xff) << 8)

		ldr   r0, .const+8   @ r0 = 0xffff
		bic   r7, r0, r7         @ r7 = (~r7) & 0xffff

		mov   r0, r11            @ r0 = frame arrary begin
		mov   r1, #1280          @ r1 = 1280
		mul   r1, r1, r9         @ r1 = 1280y (r9 = y)
		add   r0,r0,r1             @ r0 = frame + 1280y
		add   r0, r8, lsl #1     @ r0 = frame + 1280y + 2x (r8 = x)

		strh  r7, [r0]

		add r9,r9,#1	@y++
		b	loopTwo
	DoneTwo:
		add r8,r8,#1	@x++
		b loopOne


	DoneOne:
		ldmfd sp!,{r4-r11,lr}
		mov pc, lr
