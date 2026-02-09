
func(sec_func())
  println("exiting program safely");
  exit(success);
fend(abort)

func(my_func())
  println("entry of my_func");
  int *a = null;
  *a = 0xDEAD;
fend(sec_func)

churmant_main
  my_func();
churmant_mend
