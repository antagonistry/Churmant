# CHurMant
**Churmant** *(cure-ment)*, is a *compiler* for **C language**, it's used for producing *Lua-like* code, but run in the performance of the **C language**.
It also is very easy to maintain and read the header file, if you want to *contribute*. You can also call **Churmant** a *programming language*,
if you want, or you can just said *"C's Churmant Syntax"*. **NOTICE: Churmant can produce Lua-like code only, the code is still unsafe.** And here are
the *pros* and *cons* of the *language*:

### Pros
- Lua-like Syntax
- Auto Garbage-Collection

### Cons
- Unsafe Code
- No Borrow Checking
- No Smart Pointer

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

The *language* is currently in the *development* state, you can help us, if you want.
