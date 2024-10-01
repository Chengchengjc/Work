.data
  msg1:   .asciz "*****Print Name*****\n"
  msg2:   .asciz "*****End Print*****\n"
  teamNo: .asciz "Team 4\n"
  name1:  .asciz "Jay\n"
  name2:  .asciz "Chuang,Leo\n"
  name3:  .asciz "Cheng, Jessica\n"

.text
  .global name   @ Function
  .global teamNo @ String
  .global name1  @ String
  .global name2  @ String
  .global name3  @ String
  .type  name, %function
  name:
    stmfd sp!, {lr}

    ldr   r0, =msg1   @ r0 = "*****Print Name*****\n"
    bl    printf
    ldr   r0, =teamNo @ r0 = "Team 4\n"
    bl    printf
    ldr   r0, =name1  @ r0 = "Jay\n"
    bl    printf
    ldr   r0, =name2  @ r0 = "Chuang,Leo\n"
    bl    printf
    ldr   r0, =name3  @ r0 = "Cheng, Jessica\n"
    bl    printf
    ldr   r0, =msg2   @ r0 = "*****End Print*****\n""
    bl    printf
@----------------------------------------------------
    ldmfd sp!,{lr}
    mov   pc, lr

