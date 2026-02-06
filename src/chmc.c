#include "churmant.h"

churmant_main
  int CMD_SIZE = size(char) * 512;
  int OUTPUT_SIZE = size(char) * 128;
  string cmd = new(CMD_SIZE);
  string output = new(OUTPUT_SIZE);
  for(1, i < argc, 1) do
    long j = i;
    long at = -1;
    strncpy(cmd, "@gcc -O3 -ffast-math -g -I include ", CMD_SIZE);
    strncat(cmd, argv[i], CMD_SIZE);
    strncat(cmd, " -o ", CMD_SIZE);
    strncpy(output, argv[i], OUTPUT_SIZE);
    string arg = argv[j];
    for(0, arg[i] != '\0', 1) do
      if arg[i] != '.' then
        continue;
      end
      at = i;
    end
    if at == -1 then
      delete(cmd);
      delete(output);
      return(failure);
    end
    output[at] = '\0';
    println(output);
    strncat(cmd, output, CMD_SIZE);
    strncat(cmd, " -lm", CMD_SIZE);
    system(cmd);
    i = j;
  end
  delete(cmd);
  delete(output);
  return(success);
end
