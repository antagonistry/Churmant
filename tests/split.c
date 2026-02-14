churmant_main
  string raw_string = null;
  string_array my_array = null;
  allocate(raw_string, CHURMANT_LINESIZE);
  allocate_string_array(my_array);
  strncpy(raw_string, "int var = 0xFF;", CHURMANT_LINESIZE);
  split(my_array, raw_string, ' ');
  print("(");

  for(0, *my_array[i] != '\0', 1) do
    print("\"");
    print(my_array[i]);

    if *my_array[i + 1] == '\0' then
      print("\"");
      break;
    end

    print("\", ");
  end

  println(")");
churmant_mend
