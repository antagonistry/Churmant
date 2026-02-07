#include "churmant.h"

int global_argc;
int global_index;
string *global_argv;

func recover() do
  fprintf(stderr, "%s: failed to compile file '%s'\n", *global_argv, global_argv[global_index]);
  exit(failure);
end

churmant_main
  global_argc = argc;
  global_argv = argv;
  if argc == 1 then
    fprintf(stderr, "%s: no input files\n", *global_argv);
    exit(failure);
  end
  int CMD_SIZE = size(char) * 1024;
  int OUTPUT_SIZE = size(char) * 128;
  int ARG_SIZE = size(char) * 128;
  bool classic_allocation = false;
  bool classic_types = false;
  bool merge_libraries = false;
  bool debugging = false;
  if file_find("churmant_flags.txt") then
    file args = null;
    file_open(args, "churmant_flags.txt");
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
      if strcmp(arg, const(classic_allocation)) == success then
        classic_allocation = true;
        continue;
      end
      if strcmp(arg, const(classic_types)) == success then
        classic_types = true;
        continue;
      end
      if strcmp(arg, const(merge_libraries)) == success then
        merge_libraries = true;
        continue;
      end
      if strcmp(arg, const(debugging)) == success then
        debugging = true;
        continue;
      end
      printf("%s: '%s' is not a valid flag\n", *global_argv, arg);
      exit(failure);
    end
  end
  string cmd = null;
  string output = null;
  allocate(cmd, CMD_SIZE);
  allocate(output, OUTPUT_SIZE);
  for(1, i < argc, 1) do
    long j = i;
    long at = -1;
    global_index = j;
    strncpy(cmd, "gcc -O3 -ffast-math -march=native -g -I include -Werror=uninitialized ", CMD_SIZE);
    strncat(cmd, argv[i], CMD_SIZE);
    strncpy(output, argv[i], OUTPUT_SIZE);
    string arg = argv[j];
    if not file_find(arg) then
      fprintf(stderr, "%s: file '%s' does not exists\n", *global_argv, arg);
      return(failure);
    end
    for(0, arg[i] != '\0', 1) do
      if arg[i] != '.' then
        continue;
      end
      at = i;
    end
    if at == -1 then
      fprintf(stderr, "%s: '%s' is not a valid churmant source code, must be *.c\n", *global_argv, arg);
      return(failure);
    end
    if strcmp(strrchr(arg, '.'), ".c") != success then
      fprintf(stderr, "%s: '%s' is not a valid churmant source code, must be *.c\n", *global_argv, arg);
      return(failure);
    end
    if classic_allocation then
      strncat(cmd, " -Dchurmant_calloc ", CMD_SIZE);
    end
    if classic_types then
      strncat(cmd, " -Dchurmant_ctypes ", CMD_SIZE);
    end
    if merge_libraries then
      strncat(cmd, " -static ", CMD_SIZE);
    end
    if debugging then
      strncat(cmd, " -Dchurmant_debug ", CMD_SIZE);
    end
    output[at] = '\0';
    strncat(cmd, " -o ", CMD_SIZE);
    strncat(cmd, output, CMD_SIZE);
    strncat(cmd, " -lm 2>&1", CMD_SIZE);
    short ret = system(cmd);
    if ret == 127 then
      fprintf(stderr, "%s: gcc is not installed\n", *global_argv);
      return(failure);
    end
    if ret == 1 then
      fprintf(stderr, "%s: failed to compile file '%s'\n", *global_argv, arg);
      return(failure);
    end
    println(output);
    i = j;
  end
churmant_mend(recover)
