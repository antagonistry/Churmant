#include "churmant.h"

churmant_main
  if argc == 1 then
    println("no input files");
    exit(failure);
  end
  int CMD_SIZE = size(char) * 512;
  int OUTPUT_SIZE = size(char) * 128;
  int ARG_SIZE = size(char) * 128;
  bool classic_allocation = false;
  if file_find("churmant_args.txt") then
    file args = null;
    file_open(args, "churmant_args.txt");
    string arg = null;
    allocate(arg, ARG_SIZE);
    strncpy(arg, "", ARG_SIZE);
    while(true)
      string line = file_readline(arg, ARG_SIZE, args);
      if not line then
        break;
      end
      while(*arg == ' ' or *arg == '\t')
        arg++;
      end
      while(arg[strlen(arg) - 1] == ' ' or arg[strlen(arg) - 1] == '\t')
        arg[strlen(arg) - 1] = '\0';
      end
      if *arg == '#' then
        continue;
      end
      if strcmp(arg, "classic_allocation") == success then
        classic_allocation = true;
      else
        printf("'%s' is not a valid flag\n", arg);
        exit(failure);
      end
    end
  end
  string cmd = null;
  string output = null;
  allocate(cmd, CMD_SIZE);
  allocate(output, OUTPUT_SIZE);
  for(1, i < argc, 1) do
    long j = i;
    long at = -1;
    strncpy(cmd, "@gcc -O3 -ffast-math -march=native -g -I include ", CMD_SIZE);
    strncat(cmd, argv[i], CMD_SIZE);
    strncpy(output, argv[i], OUTPUT_SIZE);
    string arg = argv[j];
    for(0, arg[i] != '\0', 1) do
      if arg[i] != '.' then
        continue;
      end
      at = i;
    end
    if at == -1 then
      printf("'%s' is not a valid c source code\n", arg);
      return(failure);
    end
    if classic_allocation then
      strncat(cmd, " -Dchurment_calloc ", CMD_SIZE);
    end
    output[at] = '\0';
    println(output);
    strncat(cmd, " -o ", CMD_SIZE);
    strncat(cmd, output, CMD_SIZE);
    strncat(cmd, " -lm", CMD_SIZE);
    int_fast8_t ret = system(cmd);
    if ret != 0 then
      println("gcc is not installed");
    end
    i = j;
  end
churmant_mend
