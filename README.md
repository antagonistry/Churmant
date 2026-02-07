# CHurMant
**Churmant** *(cure-ment)*, is a *compiler* for **C language**, it's used for producing *Lua-like* code, but run in the performance of the **C language**.
It also is very easy to maintain and read the header file, if you want to *contribute*. You can also call **Churmant** a *programming language*,
if you want, or you can just said *"C's Churmant Syntax"*. And here are the *pros* and *cons* of the *language*:

### Pros
- Lua-like Syntax
- Stack Protection
- Auto Garbage-Collection

### Cons
- Is Not Object Oriented Language

If you're wondering how the *syntax* would look like in an example here it is:

```c
#include "churmant.h"

churmant_main
  int a = ~argc;
  string b = "Hello, World!";
  float c = 3.1459265;
  printf("int a = %d;\nstring b = %s;\nfloat c = %lf;\n", a, b, c);
  a = (a != argc);
  match(a)
    case(false)
      printf("a = %s;\n", const(false));
    close
    case(true)
      printf("a = %s;\n", const(true));
    close
  end
  for(0, i < 8, 1) do
    printf("i = %lli;\n", i);
  end
churmant_mend
```

The entire code that is written in **Churmant** is always *stack protected*, because whenever **SEGFAULT** happens, it quickly jumps to the recovery
part of the *code*, at the `churmant_mend(x)`; 'x' is a *function* that can be called without needing any *arguments*. **NOTICE: Any part of the code that's
not written in Churmant syntax might just straight up SEGFAULT.** The *language* is currently in the *development* state, you can help us, if you want.
