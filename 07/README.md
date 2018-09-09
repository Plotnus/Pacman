# Breaking it up

One of our current goals is to, at some point convert this to bare metal asm code.
Once we do that we will have
1. char buffer of 244x288 - this contains bytes using the 8 bit color format
2. rgb buffer of 244x288 - This is meant to imitate what we'd be writing into in assembly.
3. since were running on linux we need one additional layer that matches the display in nix


We add these one at a time. What we currently have is 3 with a quick hack for 2.
We'll want to clean this up in the process, and maybe include something for 1.


Notice that the colors aren't correct. We'll address this in the next section when we dive into endianess.


