# 03 - Display Pixels

Cool we got the framebuffer. Now lets write some data to it!

Before we begin a little cleanup is due. In the last bit we unmapped the memory. But what about the file descriptior?
We opened that? Should we close it? Yea, lets do that.
`man close` the description is quite easy

memset would be useful here as a quick test.
`man memset`
we see the signature and a descrition for `void *memset(void*s, int c, size_t n);`
lets use this in our code.
`memset(pfb,0xFF,fb\_fixed\_info.smem\_len);`

compile and run
We expect to see the screen turn white but we don't.
This is because we used `MAP_PRIVATE` in our call to memmap. `MAP_SHARED` automatically writes through to the file.
We see that for the private version that writes are not carried through to the underlying file.

Maybe we can fix that msync. `man msync` the description looks promising "syncronize a file with a memory map
`int msync(void *addr, size_t length, int flags);`
- addr - `pfb`
- length - `fb_fixed_info.smem_len`
- flags - `MS_SYNC` so we don't run ahead.

cool lets give it a go!
Wait nothing happened? Well tuns out msync was a guess because I saw it mentioned in the docs on the `MAP_SHARED` flag.
It didn't work. Lets remove that code, and give `write` a try.
`man write`

`write(fbfd, pfb, fb_fixed_info.smem_len)

We run the code and get a strange pattern. That's strange, maybe it has to do with the pitch we read about earlier.
But wait a sec! We did write to the frame buffer file!

What if we forget about mmap and just write to the fbfile when we want to display the rendered screen?
Lets try that. Get rid of all the mmap code. Leave the write code, and allocate the pfb instead.
compile and view.
Cool it did the whole thing!!

For fun try doing your own version of a double buffer. This is a technique in games.
we have two Buffers that we write to the display. Because writing to disk takes time.
This allows the engine to continue calculating the values of the pixels, and then to write to 
screen in one call.


