# 01 - our first pixel

We want to write a pixel, but where sholud we write it to?

We do have two structs, 'fb_var_screeninfo' and 'fb_fix_screeninfo'.
When we go to the header where these structs are defined (for me it is 'linux/fb.h")
, we see a comment that reads "Start of Memory Mapped I/O" and "Length of Memory Mapped I/O"
These both look promising. However, notice that it says "physical address".
Because we are operating under linux and are a "user" we don't get to write directly to physical addresses.
We need to "map" the address to a region in the memory we can access.
(Please don't believe me. Try it yourself and see what happens. What error did you get when trying to write to that address)

So, how can we map this memory?
This is where "sys/mman.h" comes into play.

Of particular interest is the function mmap, a mnuemonic for "Memory MAP".
Lets go to "sys/mman.h" file and see waht we can learn about memory mapping.

But where is it located? I don't know. So I ran `find / -name "mman.h"`
When I first ran this I got a bunch of "permission denied" so I added sudo.
This returned a bunch of files so I filtered the results using grep.
The final result was:
```
sudo find / -name "mman.h" | grep sys/mman.h
```
Which led me to "/usr/include/arm-linux/gnueabihf/sys/mman.h"
I'm going to add this to the path for my editor so I can jump to files I include in the header.
Great! now that that's setup (it takes time) lets look a the file.


notice the line
```
#define MAP_FAILED ((void*) -1)
```
This is the value we'll use to see if mmap failed.

Another declaration that looks important is
```
extern munmap (void *__addr, size_t __len) __THROW;
```
We'll have to use that when we're done with the memory.

So, looking at the mmap signature:
```
extern void *mmap (void *_addr, size_t __len, int __prot,
                   int __flags, int __fd, __off_t __offset) __THROW;
```
We will piece it together.

NOTE: your implementation of mmap may differ. That's ok. It's just about figuring out what to put in there.

1. `__addr` - this must be the physical address we are mapping `fb\_fix_bufferinfo.`
2. `__len`  - this must be the number of bytes we are mapping

On second thought, lets see if the man pages help.
```
man mmap
```
Good thing we checked! it turns out we were wrong!
From reading the manpage we gather that
1. `__addr` - the address at which to create the mapping, NULL to let the kernel decide (reccomended)
2. `__len` - the length of the mapping, the number of bytes to map
3. `__prot` - the memory protection of the mapping options are `PROT_EXEC`, `PROT_READ`, `PROT_WRITE`, `PROT_NONE`
we can or them together with '|' to arrive at any protection level we want. Observe similarities with the chmod command.
4. `__flags` - determine visibility and how updates propgate. Because this is the only program we can be greedy for now.
Lets use `MAP_PRIVATE`. It'd also be nice to draw the pixels to creen _after_ we finish updated thi buffer.
Lets see if there are other flags that may be useful. We see something about msync.
That may be the call we need to display the changes.
5. `__fd` - the file descriptior
7. `__offset` - we have no need for an offset
 Using the above information we have decided that
 `_addr    := NULL`
 `_len     := fb_variable_info.smem_len`
 `__prot   := PROT_READ`
 `__flags  := MAP_PRIVATE | MAP_UNINITIALIZED`
 `__fd     := fbfd`
 `__offset := 0`

 Now, lets code this up and see what happens.

 Note: I don't know about you, but I'm starting to realize that alot of coding is figuring stuff out!


 Oh, but wait! We forgot to unmap the memory!! yikes! This is a resource leak and can bring your
 system to a crawl!

 Lets add that unmap then reboot our systems to make sure all memory is freed.
 `man unmap` and then we know to add `munmap(pfb,fb_size)` to free the mapped memory
 


We we tried writing to the address returned from our framebuffer code we would get a segmentation fault.
This is because we can not write to those memory locations. We need to map to a region we can write to.
This is what mmap is for. 




