/* wanted a memset for 4 byte patterns */

@ r0 : the destination 
@ r1 : the 32bit pattern to write
@ r2 : the number of 32-bit words to write
.global memset4
memset4:
  mov r3,r0
  memset4_loop:
  cmp r2,#0
  beq return
  str r1,[r3]
  sub r2,r2,#1
  add r3,r3,4
  bal memset4_loop

  return:
  mov pc,lr
   

