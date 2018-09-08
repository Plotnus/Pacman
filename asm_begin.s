.section .text
.global _ASM_BEGIN
_ASM_BEGIN:
  @ save lr
  push {lr}
  @ get the fb address
  mov lr,pc
  b FB_GetAddr
  @ set the color
  mov r3,#0xFFFF 
  lsl r3,r1,16
  orr r3,r1,0xFF
  @ make the call
@ mov lr,pc
@ b _RenderRow

  @ drawRect
  fb .req r0
  color .req r1
  x .req r2
  y .req r3
  w .req r4
  h .req r5
  pxPerRow .req r6
  mov x,#122
  mov y,#144
  mov w,#48
  mov h,#64
  mov pxPerRow,#244
  push {h,pxPerRow}
  @make a cool color
  mov r1,#0xFF00
  lsl r1,r1,#16
  add r1,r1,#0xFF00
  add r1,r1,#0x00FF
  @ make the call
  mov lr,pc
  bl DrawRect

  @ return
  pop {pc};


@ r0 - fb start address
@ r1 - iCol
@ r2 - iRow
@ r3 - color
_RenderRow:
  push {lr}
  mov r5,288      @ r5 <= iRow
  mov r6,r3
  drawRow:
    @ loop control
    cmp r5,#0
    popeq {pc}   @ <= return if done
    sub r5,r5,#1  @ <= decrement counter
    @ prep for memset call
    mov r1,r6     @ set color
    mov r2,#244   @ num words to write
    @ make the proc call
    mov lr,pc     @ set lr so we can return
    b memset4     @ make the call
    @ increment fb
    mov r6,#1824    @ r6 <= physical screen width
    lsl r6,#2       @ 1824 * 4
    add r0,r0,r6    @ TODO non hardcoded
    @ repeat the loop
    b drawRow

@ r0 - fb
@ r1 - color
@ r2 - x
@ r3 - y
@ r4 - width
@ s0 - height   
@ s1 - pxPerRow  <= push 
DrawRect:
  pop {r5,r6}   @ that's nice how this works
  push {lr}
  @ calculate starting offset
  mul r7,r2,r6  @ r7 := y * pxPerRow
  add r7,r7,r1  @ r7 := previous + x
  lsl r7,r7,#2  @ numPixels * nBytesPerPx
  mov r8,r6, lsl #2 @ r2 := nBytesPerRow
  @ set fb to start of rect
  add r0,r0,r7  @ fb += offset
  mov r2,r4     @ r2 := width
  drawRectRow:
    cmp r5,#0   @ compare height to 0
    popeq {pc}  @ return
    @ make call to memset
    mov lr,pc
    b memset4
    @ mutate looping variables
    sub r5,r5,#1 @ height -= 1
    lsl r9,r6,#2 @ r9 := pxPerRow * 4
    add r0,r0,r9 @ fb += pxPerRow * 4
    b drawRectRow
    

.section .data
.align 4

.global nRows
nRows: .int 288
.global nCols
nCols: .int 244

