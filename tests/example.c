#include "churmant.h"

function main(integer argc, string argv[]) do
  integer a = ~argc;
  string b = "Hello, World!";
  float c = 3.1459265;
  printf("integer a = %d;\nstring b = %s;\nfloat c = %lf;\n", a, b, c);
  a = (a != argc);
  match(a)
    case(false)
      printf("a = %s;\n", constant(false));
    close
    case(true)
      printf("a = %s;\n", constant(true));
    close
  end
  for(0, i < 8, 1) do
    printf("i = %lu;\n", i);
  end
  return(successful);
end
