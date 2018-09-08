.section .data
.align 16
@ ten words, fourty bytes
.global framebuffer
framebuffer:
  .unsigned int 244 @ 0x00 @ width
  .unsigned int 288 @ 0x04 @ height
  .unsigned int 244 @ 0x08 @ virtual width
  .unsigned int 288 @ 0x0C @ virtual height
  .unsigned int 0   @ 0x10 @ pitch - bytes between each row - set by GPU
  .unsigned int 24  @ 0x14 @ depth - number of bits per pixel
  .unsigned int 0   @ 0x18 @ offset_x
  .unsigned int 0   @ 0x1C @ offset_y
  .unsigned int 0   @ 0x20 @ pixelDataAddr - pointer to the frame buffer pixel data
  .unsigned int 0   @ 0x24 @ number of bytes in pixel data

@ Set by GPU are offsets, 10h, 20h, 24h
