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
