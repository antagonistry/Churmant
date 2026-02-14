#define CMD_SIZE size(char) * 1024
#define OUTPUT_SIZE size(char) * 128
#define ARG_SIZE size(char) * 128
#define LINE_SIZE size(char) * 1024
#define ALLOCATED_BUFFER_SIZE size(char) * 512
#define UNALLOCATED_BUFFER_SIZE size(char) * 512
#define ALLOCATED_SIZE 512
#define UNALLOCATED_SIZE 512
#define PARTS_BUFFER_SIZE size(char) * 256
#define PARTS_SIZE 256
#define COMMANDS_SIZE size(char) * 8192
#define LIBS_SIZE size(char) * 4096

bool manual_allocation = false;
bool classic_types = false;
bool debugging = false;
bool native_performance = false;
bool reduce_binary_size = false;
bool binary_readability = false;
string libs = null;

void parsing_flags() do
  println("(churmant/compiler) parsing flags");
  
  if not file_find("~/.Churmant/churmant_flags.txt") then
    println("(churmant/compiler) finished parsing flags");
    return;
  end

  file args = null;
  file_open(args, "~/.Churmant/churmant_flags.txt");
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
    
    while(arg[len(arg) - 1] == ' ' or arg[len(arg) - 1] == '\t')
      arg[len(arg) - 1] = '\0';
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
end

bool validating_source(string arg) do
  printf("(churmant/compiler) validating source file '%s'\n", arg);
  file source = null;
  file_open(source, arg);
  string line = null;
  allocate(line, LINE_SIZE);
  file_write("._chmp.c", "");
  char allocated[ALLOCATED_SIZE][ALLOCATED_BUFFER_SIZE];
  char unallocated[ALLOCATED_SIZE][ALLOCATED_BUFFER_SIZE];
  short allocated_i = 0;
  short unallocated_i = 0;
  long line_number = 0;
  bool parsing_normal = false;
  bool parsing_if = false;
  bool parsing_else = false;
  bool parsing_string = false;
  bool parsing_case = false;
  bool single_comment = false;
  bool multi_comment = false;
  bool fatal = false;
  int string_at = -1;
  int string_end = -1;
  int called_main = 0;

  while(true)
    strncpy(line, "", LINE_SIZE);
    string tmp = file_readline(line, LINE_SIZE, source);
    int semis = 0;
    line_number++;
    string_at = -1;
    string_end = -1;
    single_comment = false;
    
    if not tmp then
      break;
    end
    
    if len(line) >= LINE_SIZE then
      fprintf(stderr, "(churmant/compiler) line %lli is too big\n", line_number);
      fatal = true;
    end

    long line_len = len(line); 

    string temp = null;
    allocate(temp, LINE_SIZE);
    strncpy(temp, line, LINE_SIZE);
    strncat(temp, "\n", LINE_SIZE);

    if strncmp(line, temp, len(line) - 1) != success then
      continue;
    end

    file_append("._chmp.c", temp);
    char parts[PARTS_SIZE][PARTS_BUFFER_SIZE];
    short parts_i = 0;
    parsing_string = false;

    for(0, i < line_len, 1) do
      if line[i] == ';' and not parsing_string then
        semis++;
      end

      if line[i] == '\"' then
        if i - 1 > 0 then
          if line[i - 1] != '\\' then
            parsing_string = not parsing_string;
            continue;
          end

          continue;
        end

        parsing_string = not parsing_string;

        if parsing_string then
          string_at = i;
        else
          string_end = i;
        end
      end

      if parsing_string then
        continue;
      end

      if line[i] == '/' and line[i + 1] == '/' then
        single_comment = true;
        line[i] = '\0';
        break;
      end

      if line[i] == '-' and line[i + 1] == '-' then
        single_comment = true;
        line[i] = '\0';
        break;
      end

      if line[i] == '#' then
        single_comment = true;
        line[i] = '\0';
        break;
      end
    end

    if parsing_string then
      fprintf(stderr, "(churmant/compiler) line %lli, unterminated string\n", line_number);
      fatal = true;
    end

    while(not (*line >= 'a' and *line <= 'z') and not (*line >= 'A' and *line <= 'Z') and *line != '_' and *line != '*' and *line != '\"' and *line != '\'')
      line++;
    end

    for(0, line[i] != '\0', 1) do
      if line[i] != ';' then
        continue;
      end

      line[i] = '\0';
      break;
    end

    if string_at != -1 then
      string temp = null;
      string original = null;
      allocate(temp, LINE_SIZE);
      allocate(original, LINE_SIZE);
      short j = 0;

      while(strchr(line, ' '))
        strncpy(temp, line, LINE_SIZE);
        
        for(0, temp[i] != ' ', 1) do
          j = i;
        end

        temp[j + 1] = '\0';
        strncpy(original, temp, LINE_SIZE);

        while(*temp == ' ' or *temp == '\t')
          temp++;
        end

        if strchr(temp, '\"') then
          parsing_string = not parsing_string;
        end

        while(temp[len(temp) - 1] == ' ' or temp[len(temp) - 1] == '\t')
          temp[len(temp) - 1] = '\0';
        end

        line += len(original);

        if *temp == '\0' then
          continue;
        end

        if parts_i == PARTS_SIZE then
          fprintf(stderr, "(churmant/compiler) line %lli, too many commands for a single line of code\n", line_number);
          fatal = true;
        end

        strncpy(parts[parts_i], temp, PARTS_BUFFER_SIZE);
        parts_i++;
      end

      parsing_string = false;
      strncpy(line, "", LINE_SIZE);

      for(0, i < parts_i, 1) do
        if strchr(parts[i], '\"') then
          continue;
        end

        strncat(line, parts[i], LINE_SIZE);
      end
    end

    if semis > 1 then
      fprintf(stderr, "(churmant/compiler) line %lli, too many commands for a single line of code\n", line_number);
      fatal = true;
    end

    if strstr(line, "/*") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'C' style multi-comment\n", line_number);
      fatal = true;
    end

    if strstr(line, "main") then
      called_main++;
      
      if called_main > 1 then
        fprintf(stderr, "(churmant/compiler) line %lli, 'main' function cannot be called more than once\n", line_number);
        fatal = true;
      end
    end

    if strstr(line, "char") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'char' type, use 'int' instead\n", line_number);
      fatal = true;
    end

    if strstr(line, "*") and strstr(line, "=") and not strstr(line, "null") then
      if strncmp(line, "int", len("int")) == success or
          strncmp(line, "long", len("long")) == success or 
          strncmp(line, "bool", len("bool")) == success or
          strncmp(line, "byte", len("byte")) == success or
          strncmp(line, "string", len("string")) == success then
        fprintf(stderr, "(churmant/compiler) line %lli, pointer always need to be initialized as 'null'\n", line_number);
        fatal = true;
      end
    end

    if strstr(line, "string_array") and strstr(line, "=") and not strstr(line, "null") then
      fprintf(stderr, "(churmant/compiler) line %lli, pointer always need to be initialized as 'null'\n", line_number);
      fatal = true;
    end

    if strstr(line, "allocate") then
      while(*line != 'a')
        line++;
      end

      if strncmp(line, "allocate", len("allocate")) != success and strstr(line, "(") and strstr(line, ")") then
        print(line);
        fprintf(stderr, "(churmant/compiler) line %lli, invalid command pattern\n", line_number);
        fatal = true;
        continue;
      end

      line += len("allocate");

      while(not (*line >= 'a' and *line <= 'z') and not (*line >= 'A' and *line <= 'Z') and *line != '_')
        line++;
      end

      for(0, line[i] != '\0', 1) do
        if i != ',' then
          continue;
        end
        
        line[i] = '\0';
        break;
      end

      long j = 0;

      for(0, (line[i] >= 'a' and line[i] <= 'z') or (line[i] >= 'A' and line[i] <= 'Z') or line[i] == '_', 1) do
        j = i;
      end

      for(0, i < unallocated_i, 1) do
        if not strstr(line, allocated[i]) then
          continue;
        end

        strncpy(unallocated[i], "", UNALLOCATED_BUFFER_SIZE);
        break;
      end

      line[j + 1] = '\0';
      strncpy(allocated[allocated_i], line, ALLOCATED_BUFFER_SIZE);
      allocated_i++;
    end

    for(0, i < unallocated_i, 1) do
      if *unallocated[i] == '\0' then
        continue;
      end

      long j = 0;

      for(0, strncmp(line + i, unallocated[i], len(unallocated[i])) != success, 1) do
        j = i;
      end

      string before = null;
      allocate(before, LINE_SIZE);
      strncpy(before, line, LINE_SIZE);
      before[j] = '\0';
      int last_ch = before[len(before) - 1];

      if *before != '\0' then
        if (last_ch >= 'a' and last_ch <= 'z') or (last_ch >= 'A' and last_ch <= 'Z') or last_ch == '_' then
          continue;
        end
      end

      line += len(before);
      int unallocated_len = len(unallocated[i]);
      last_ch = line[unallocated_len + 2];

      if (last_ch >= 'a' and last_ch <= 'z') or (last_ch >= 'A' and last_ch <= 'Z') or last_ch == '_' then
        continue;
      end

      if not strstr(line, unallocated[i]) then
        continue;
      end
      
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use unallocated pointer\n", line_number);
      fatal = true;
    end

    if strstr(line, "null") and strstr(line, "=") then
      long j = 0;

      while(not (*line >= 'a' and *line <= 'z') and not (*line >= 'A' and *line <= 'Z') and *line != '_')
        line++;
      end

      for(0, strncmp(line + i, "null", len("null")) != success, 1) do
        j = i;
      end

      line[j] = '\0';

      for(0, line[i] != '=', 1) do
        j = i;
      end

      line[j] = '\0';
      j = 0;

      for(0, line[i] != '\0', 1) do
        if line[i] != ' ' then
          continue;
        end

        j++;
      end

      if j > 0 then
        while(*line != ' ')
          line++;
        end

        line++;

        for(0, (line[i] >= 'a' and line[i] <= 'z') or (line[i] >= 'A' and line[i] <= 'Z') or line[i] == '_', 1) do
          j = i;
        end

        line[j + 1] = '\0';

        while(*line == '*')
          line++;
        end

        strncpy(unallocated[unallocated_i], line, UNALLOCATED_BUFFER_SIZE);
        unallocated_i++;
        continue;
      end

      for(0, (line[i] >= 'a' and line[i] <= 'z') or (line[i] >= 'A' and line[i] <= 'Z') or line[i] == '_', 1) do
        j = i;
      end 

      line[j + 1] = '\0';
      j = false;
     
      for(0, i < ALLOCATED_SIZE, 1) do
        if strcmp(line, allocated[i]) != success then
          continue;
        end

        j = true;
        fprintf(stderr, "(churmant/compiler) line %lli, allocated pointer cannot be pointed to null\n", line_number);
        fatal = true;
        break;
      end

      if not j then
        fprintf(stderr, "(churmant/compiler) line %lli, unallocated pointer cannot be pointed to null\n", line_number);
        fatal = true;
      end
    end

    if strstr(line, "void") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'void' type, use 'ptr' instead\n", line_number);
      fatal = true;
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

    if strstr(line, "longjmp") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'longjmp' function\n", line_number);
      fatal = true;
    end

    if strstr(line, "setjmp") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'setjmp' function\n", line_number);
      fatal = true;
    end

    if strstr(line, "free") then
      fprintf(stderr, "(churmant/compiler) line %lli, cannot use 'free' function\n", line_number);
      fatal = true;
    end

    if strstr(line, "if") then
      parsing_if = true;
    end

    if strstr(line, "case") or strstr(line, "default") then
      if parsing_case then
        fprintf(stderr, "(churmant/compiler) line %lli, cannot stack 'case' or 'default' conditions inside each other\n", line_number);
        fatal = true;
      end

      parsing_case = true;
    end

    if strstr(line, "close") then
      parsing_case = false;
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
    return(false);
  end

  printf("(churmant/compiler) finished validating source file '%s'\n", arg);
  return(true);
end

void allowing_flags(string cmd) do
  println("(churmant/compiler) allowing flags");
  if manual_allocation then
    strncat(cmd, " -Dchurmant_malloc ", CMD_SIZE);
  end
  
  if classic_types then
    strncat(cmd, " -Dchurmant_ctypes ", CMD_SIZE);
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
end

void resetting_flags() do
  println("(churmant/compiler) resetting flags");
  manual_allocation = false;
  classic_types = false;
  debugging = false;
  native_performance = false;
  reduce_binary_size = false;
  binary_readability = false;
  println("(churmant/compiler) finished resetting flags");
end

void version() do
  printf("Churmant Compiler [%s] version %.2lf\n", __FILE__, CHURMANT_VERSION);
end

void help() do
  println("Churmant Help\n");
  println("usage: chmc [options] files\n");
  println("options:");
  println("  --version");
  println("  --help\n");
  println("flags ('churmant_flags.txt' file):");
  println("  manual_allocation");
  println("  classic_types");
  println("  debugging");
  println("  native_performance");
  println("  reduce_binary_size");
  println("  binary_readability\n");
end

churmant_main
  if argc == 1 then
    fprintf(stderr, "(churmant/compiler) no input files\n");
    exit(failure);
  end 
  
  string cmd = null;
  string output = null;
  string line = null;
  allocate(cmd, CMD_SIZE);
  allocate(output, OUTPUT_SIZE);
  allocate(line, LINE_SIZE);
  allocate(libs, LIBS_SIZE);
  parsing_flags();
  
  for(1, i < argc, 1) do 

    if strcmp(argv[i], "--version") == success then
      version();
      continue;
    end

    if strcmp(argv[i], "--help") == success then
      help();
      continue;
    end

    long j = i;
    long at = -1;
    strncpy(cmd, "gcc -O3 -g3 -static-libgcc -Wl,-rpath=. -L. --include=include/churmant.h -Werror=uninitialized -Werror=return-local-addr -std=c99 ._chmp.c", CMD_SIZE);
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
    
    if len(cmd) + len(output) >= CMD_SIZE then
      fprintf(stderr, "(churmant/compiler) output file name is too big\n");
      continue;
    end

    if len(cmd) + len(libs) >= CMD_SIZE then
      fprintf(stderr, "(churmant/compiler) too many libraries to link\n");
      continue;
    end 
   
    strncat(cmd, libs, CMD_SIZE);
    strncat(cmd, " -o ", CMD_SIZE);
    strncat(cmd, output, CMD_SIZE);
    strncat(cmd, " 2>&1", CMD_SIZE);
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
  end
churmant_mend
