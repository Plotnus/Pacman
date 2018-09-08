
## section 1 adding a makefile
Lets add a makefile to make our lives easier.
This is a very basic makefile.
```
all:
    cc main.c

clean:
    rm a.out
```
In makefiles white space is significant. Make sure that there is a 'tab' character before each command.

`cc main.c` compiles the source file main.c and writes the executable to a.out, the default output name.

the command `make clean` should clean up our workspace. It will remove files that were generated during compilation.
We know that the only file that will be generated is 'a.out'. So, we will remove that with `rm a.out`


Lets test this makefile out before moving on.
```
pi$ ls
```
Observe the files that are listed.
```
pi$ make all
```
Note the comand that is run. Does this look familiar?
```
pi$ ls
```
Notice what has changed from the first ls.

```
pi$ ./a.out
```
Cool this is the same output as where we left off.
```
pi$ make clean
```
Note the comand that is run. Does this look familiar?
```
pi$ ls
```
Notice anything different from the previous output for the ls command?


Keep in mind we could of been fine just running the two commands we set up in the makefile.
Using a makefile can make our lives easier & that's why we use them.
