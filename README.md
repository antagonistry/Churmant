
<div align="center">
    <img src="images/social.png"/>
</div>

<!--
# Churmant
**Churmant** *(cure-ment)*, is a *compiler* for **C** *language*, it's used for producing *Lua-like* code, but also run in a *blazingly* fast speed, when compared to others.
It also is very easy to maintain and read the header file, if you want to *contribute*. The reason why **Churmant** is its own *language*, is because its *syntax*
are not like **C** entirely, like `normal` *keyword* cannot be *stack* inside each other, for example.
-->

## Why Churmant

- **Lua-like Syntax:** Easy to read and write the code.
- **Stack's "Second Life:** Second life to handle the undetected error.
- **Safe Heap Allocation:** No more confusion about `malloc`, `realloc` or `calloc`.
- **Auto Garbage-Collection:** Automatically frees the heap-sided memories.
- **Reusing Leftovers:** Reuse leftovers from the garbage without any problems.

If you are wondering how would the *syntax* look like, here's an example of an *error* that would be caught on the *compiler*.

<div style="font: monospace; border: 8px rgb(54, 60, 66); border-radius: 4px; padding: 16px; width: 100%; background-color: rgb(54, 60, 66); color: rgb(255, 255, 255);">
    <span style="color: rgb(227, 216, 136);">churmant_main</span>
    <br>
    &emsp;<span style="color: rgb(136, 188, 227);">int</span>
    { color: rgb(136, 188, 227); }
    <span class="point">*</span>pointer
    =
    <span style="color: rgb(227, 136, 136);">null</span>;
    <br>
    &emsp;<span style="color: rgb(199, 199, 199);">*</span>pointer
    =
    <span style="color: rgb(227, 136, 136);">0xDEAD</span>;
    <span style="color: rgb(133, 133, 133);"># line 3, cannot use unallocated pointer</span>
    <br>
    <span style="color: rgb(227, 216, 136);">churmant_mend</span>
</div>

## Installation Steps

To *install* the **Churmant** *repository* and *compile* the *source* files, you need to download `chminit.exe` first, which you can *install* from this
[link](https://sourceforge.net/projects/churmant/files/chminit.exe/download). Then type `y` in every question. After the *installation*, you might want to *insert* an
*environment path* to the *repository*.
