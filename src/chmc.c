final int CMD_SIZE = size(char) * 1024;
final int OUTPUT_SIZE = size(char) * 128;
final int ARG_SIZE = size(char) * 128;
final int LINE_SIZE = size(char) * 1024; 
final int UNALLOCATED_SIZE = size(char) * 1024; 
bool manual_allocation = false;
bool classic_types = false;
bool merge_libraries = false;
bool debugging = false;
bool native_performance = false;
bool reduce_binary_size = false;
bool binary_readability = false;

func(parsing_flags())
  println("(churmant/compiler) parsing flags");
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
    
    if strcmp(arg, const(manual_allocation)) == success then
      manual_allocation = true;
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
    
    if strcmp(arg, const(native_performance)) == success then
      native_performance = true;
      continue;
    end
    
    if strcmp(arg, const(reduce_binary_size)) == success then
      reduce_binary_size = true;
      continue;
    end
    
    if strcmp(arg, const(binary_readability)) == success then
      binary_readability = true;
      continue;
    end
    
    fprintf(stderr, "(churmant/compiler) '%s' is not a valid flag\n", arg);
    exit(failure);
  end
  println("(churmant/compiler) finished parsing flags");
fend(abort)

func(validating_source(string arg))
  printf("(churmant/compiler) validating source file '%s'\n", arg);
  file source = null;
  file_open(source, arg);
  string line = null;
  allocate(line, LINE_SIZE);
  long line_number = 0;
  bool parsing_normal = false;
  bool parsing_if = false;
  bool parsing_else = false;
  bool parsing_string = false;
  bool single_comment = false;
  bool multi_comment = false;
  bool fatal = false;
  int multi_comment_at = -1;
  int multi_comment_end = -1;
  long cursor = 0;

  while(true)
    strncpy(line, "", LINE_SIZE);
    string tmp = file_readline(line, LINE_SIZE, source);
    long len = strlen(line);
    line_number++;
    single_comment = false;
    
    if not tmp then
      break;
    end
    
    if line[LINE_SIZE - 1] != '\0' then
      println(line);
      fprintf(stderr, "(churmant/compiler) line %lli's command(s) are too big\n", line_number);
      exit(failure);
    end

    for(0, i < len, 1) do
      cursor++;
      if line[i] == '\"' then
        parsing_string = not parsing_string;
      end

      if parsing_string then
        continue;
      end

      if line[i] == '/' and line[i + 1] == '/' then
        single_comment = true;
        line[i] = '\0';
        break;
      end

      /*if line[i] == '-' and line[i + 1] == '-' then
        single_comment = true;
        line[i] = '\0';
        break;
      end*/

      if line[i] == '/' and line[i + 1] == '*' then
        multi_comment = true;
        multi_comment_at = i;
        i++;
        continue;
      end

      if line[i] == '*' and line[i + 1] == '/' then
        i++;
        multi_comment_end = ++i;
        continue;
      end

      /*if line[i] == '#' then
        single_comment = true;
        line[i] = '\0';
        break;
      end*/
    end

    if multi_comment and multi_comment_end == -1 then
      line[multi_comment_at] = '\0';
      multi_comment_at = 0;
    end

    if multi_comment and multi_comment_end != -1 then
      line += multi_comment_end;
      multi_comment_at = -1;
      multi_comment_end = -1;
    end

    if parsing_string then
      continue;
    end

    if strstr(line, "malloc") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'malloc' function, use 'allocate' instead\n", line_number);
      fatal = true;
    end

    if strstr(line, "calloc") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'calloc' function, use 'allocate' instead\n", line_number);
      fatal = true;
    end

    if strstr(line, "realloc") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'realloc' function, use 'allocate' instead\n", line_number);
      fatal = true;
    end

    if strstr(line, "free") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'free' function\n", line_number);
      fatal = true;
    end

    if strstr(line, "if") then
      parsing_if = true;
    end

    if strstr(line, "normal") then
      if parsing_normal then
        fprintf(stderr, "(churmant/compiler) line %lli, cannot stack 'normal' statements inside each other\n", line_number);
        fatal = true;
      end

      parsing_normal = true;
    end

    if strstr(line, "else") then
      if parsing_if then
        parsing_if = false;
        parsing_else = true;
      else
        if parsing_normal then
          fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'else' keyword in 'normal' statement, use 'error' instead\n", line_number);
          fatal = true;
        end
      end
    end

    if strstr(line, "end") then
      if parsing_normal then
        parsing_normal = false;
      end

      if parsing_if then
        parsing_if = false;
      end

      if parsing_else then
        parsing_else = false;
      end
    end
  end

  if fatal then
    return false;
  end
  printf("(churmant/compiler) finished validating source file '%s'\n", arg);
fend(abort)

func(allowing_flags(string cmd))
  println("(churmant/compiler) allowing flags");
  if manual_allocation then
    strncat(cmd, " -Dchurmant_malloc ", CMD_SIZE);
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
  
  if native_performance then
    strncat(cmd, " -march=native ", CMD_SIZE);
  end
  
  if reduce_binary_size then
    strncat(cmd, " -Os ", CMD_SIZE);
  end
  
  if binary_readability then
    strncat(cmd, " -ffunction-sections -fdata-sections ", CMD_SIZE);
  end
  println("(churmant/compiler) finished allowing flags");
fend(abort)

func(resetting_flags())
  println("(churmant/compiler) resetting flags");
  manual_allocation = false;
  classic_types = false;
  merge_libraries = false;
  debugging = false;
  native_performance = false;
  reduce_binary_size = false;
  binary_readability = false;
  println("(churmant/compiler) finished resetting flags");
fend(abort)

churmant_main
  if argc == 1 then
    fprintf(stderr, "(churmant/compiler) no input files\n");
    exit(failure);
  end
  
  if file_find("churmant_flags.txt") then
    parsing_flags();
  end
  
  string cmd = null;
  string output = null;
  string line = null;
  allocate(cmd, CMD_SIZE);
  allocate(output, OUTPUT_SIZE);
  allocate(line, LINE_SIZE);
  
  for(1, i < argc, 1) do
    long j = i;
    long at = -1;
    strncpy(cmd, "gcc -O3 -g3 --include=include/churmant.h -Werror=uninitialized -Werror=return-local-addr -std=c99 ", CMD_SIZE);
    strncat(cmd, argv[i], CMD_SIZE);
    strncpy(output, argv[i], OUTPUT_SIZE);
    string arg = argv[j];
    
    if not file_find(arg) then
      fprintf(stderr, "(churmant/compiler) file '%s' does not exists\n", arg);
      continue;
    end
    
    for(0, arg[i] != '\0', 1) do
      if arg[i] != '.' then
        continue;
      end
      at = i;
    end
    
    if at == -1 then
      fprintf(stderr, "(churmant/compiler) '%s' invalid file type, must be *.c\n", arg);
      continue;
    end
    
    if strcmp(strrchr(arg, '.'), ".c") != success then
      fprintf(stderr, "(churmant/compiler) '%s' invalid file type, must be *.c\n", arg);
      continue;
    end

    output[at] = '\0';
    
    if not validating_source(arg) then
      continue;
    end

    allowing_flags(cmd);

    if cmd[CMD_SIZE - 1] != '\0' or output[OUTPUT_SIZE - 1] != '\0' then
      fprintf(stderr, "(churmant/compiler) output file name is too big\n");
      continue;
    end 
    
    strncat(cmd, " -o ", CMD_SIZE);
    strncat(cmd, output, CMD_SIZE);
    strncat(cmd, " -lpthread -lm 2>&1", CMD_SIZE);
    printf("(churmant/compiler) compiling source file '%s'\n", arg);
    short ret = system(cmd);
    
    if ret == 127 then
      fprintf(stderr, "(churmant/compiler) 'gcc' compiler is not installed\n");
      continue;
    end
    
    if ret == 1 then
      fprintf(stderr, "(churmant/compiler) failed to compile source file '%s'\n", arg);
      continue;
    end 
    
    printf("(churmant/compiler) compiled, binary at '%s'\n", output);
    i = j;
    resetting_flags();
  end
churmant_mend
