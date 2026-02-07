#include "churmant.h"

func recover() do
  println("'a' is a pointer");
end

churmant_main
  int *a = null;
  printf("%d\n", *a);
churmant_mend(recover)
