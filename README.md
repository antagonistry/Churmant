<div align="center">
    <img src="images/logo.png"/>
</div>

# Churmant
**Churmant** *(cure-ment)*, is a *compiler* for **C** *language*, it's used for producing *Lua-like* code, but also run in a *blazingly* fast speed, when compared to others.
It also is very easy to maintain and read the header file, if you want to *contribute*. The reason why **Churmant** is its own *language*, is because its *syntax*
are not like **C** entirely, like `normal` *keyword* cannot be *stack* inside each other, for example.

### Why Churmant
- Lua-like Syntax
- Stack's "Second Life"
- Safe Heap Allocation
- Auto Garbage-Collection

If you're wondering how the *syntax* would look like in an example here it is:

```c
churmant_main
    int *pointer = null;
    *pointer = 0xDEAD; # huh, well; its's still not segfault
churmant_mend
```

### Installation Steps
To *install* the **Churmant** *repository* and *compile* the *source* files, you need to download `chminit.exe` first, which you can *install* from this
[link](https://sourceforge.net/projects/churmant/files/chminit.exe/download). Then type `y` in every question. After the *installation*, you might want to *insert* an
*environment path* to the *repository*.

The entire code that is written in **Churmant**, its *stack* has *"second life"*, because whenever **segmentation fault** happens, it quickly jumps to the recovery
part of the *code*. The best practice for the *"second life"* is just to create another *strategy* to handle the issue. The *language* would be entirely safe, ***IF*** you
use `bin/chmc` *compiler* and not the default **GCC** one. Basically, **Churmant** *compiler* is just a **GCC** *compiler* with some topping.
