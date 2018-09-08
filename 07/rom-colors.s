/* use the colors index as an offset from base */
.section data
.global colors  @ 128 bytes, 4 words
.char 0   @ 0x00 @ #0x00
.char 7   @ 0x07 @ #
.char 102 @ 0x66 @ #
.char 239 @ 0xEF @ #

.char 0   @ 0x00 @ #0x04
.char 248 @ 0xF8 @ #
.char 234 @ 0xEA @ #
.char 111 @ 0x6F @ #

.char 0   @ 0x00 @ #0x08
.char 63  @ 0x3F @ #
.char 0   @ 0x00 @ #
.char 201 @ 0xC9 @ #

.char 56  @ 0x38 @ #0x0C
.char 170 @ 0xAA @ #
.char 175 @ 0xAF @ #
.char 246 @ 0xF6 @ #

/* to _really_ match the rom add 16 bytes of 0 */
