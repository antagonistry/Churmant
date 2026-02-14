#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdalign.h>
#include <stdarg.h>

#ifndef __CHURMANT_H
#define __CHURMANT_H

#ifndef churmant_ctypes
  #define int int_fast32_t
  #define long int_fast64_t
  #define float double
  #define short int_fast16_t
#endif

#define tiny int_fast8_t
#define string char*
#define string_array char**
#define bool _Bool
#define byte _Bool
#define char char
#define inline inline
#define align alignas(32)
#define ptr intptr_t
#define std_ptr void*
#define tuple ptr*
#define file FILE*
#define true 1
#define false 0
#define ok true
#define null NULL
#define success 0
#define failure 1
#define not !
#define or ||
#define and &&
#define if if (
#define then ) {
#define end };
#define do {
#define else } else {
#define match(x) switch(x) {
#define case(x) case(x): {
#define close } break;
#define default default: {
#define while(x) while(x) {
#define for(x, y, z) for(long i = x; y; i += z)
#define normal if not setjmp(churmant_buffer) then
#define error else
#define print(x) fputs(x, stdout)

#define func(x) \
ptr x do \
  signal(SIGINT, churmant_signal); \
  signal(SIGSEGV, churmant_signal); \
  normal

#define fend(x) \
  error \
    x(); \
  end \
end

#define println(x) \
fputs(x, stdout); \
fputc('\n', stdout)

#define const(x) #x
#define final const

#define exit(x) \
  for(0, i < churmant_findex, 1) do \
    if not churmant_files[i] then \
      continue; \
    end \
    fclose(churmant_files[i]); \
  end \
  for(0, i < churmant_dindex, 1) do \
    if churmant_dynamics[i] != null then \
      continue; \
    end \
    free(churmant_dynamics[i]); \
  end \
  exit(x)

#define union(x) union x {
#define struct(x) struct x {
#define enum(x) enum x {

#define allocate(x, y) \
if x != null then \
  println("(churmant) allocated pointer"); \
  longjmp(churmant_buffer, CHURMANT_JUMP); \
end \
x = calloc(1, y); \
churmant_dynamics[churmant_dindex] = x; \
churmant_dindex++; \
if churmant_dindex > CHURMANT_MAXDYNAMICS then \
  println("(churmant) dynamics overflows"); \
  exit(failure); \
end \
x

#define allocate_string_array(x) \
if x != null then \
  println("(churmant) allocated pointer"); \
  longjmp(churmant_buffer, CHURMANT_JUMP); \
end \
x = calloc(CHURMANT_ARRAYSIZE, size(char*)); \
churmant_dynamics[churmant_dindex] = x; \
churmant_dindex++; \
if churmant_dindex > CHURMANT_MAXDYNAMICS then \
  println("(churmant) dynamics overflows"); \
  exit(failure); \
end \
for(0, i < CHURMANT_ARRAYSIZE, 1) do \
  x[i] = calloc(CHURMANT_ARRAYBUFFERSIZE, size(char)); \
  churmant_dynamics[churmant_dindex] = x[i]; \
  churmant_dindex++; \
  if churmant_dindex > CHURMANT_MAXDYNAMICS then \
    println("(churmant) dynamics overflows"); \
    exit(failure); \
  end \
end \
x

#define size(x) sizeof(x)
#define type(x) typeof(x)

#define file_open(x, y) \
if x then \
  println("(churmant) allocated pointer"); \
  longjmp(churmant_buffer, CHURMANT_JUMP); \
end \
x = fopen(y, "r+"); \
if not x then \
  x = fopen(y, "w+"); \
  if not x then \
    println("(churmant) file not found / invalid path"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
end \
churmant_files[churmant_findex] = x; \
churmant_findex++; \
if churmant_findex > CHURMANT_MAXFILES then \
  println("(churmant) files overflows"); \
  exit(failure); \
end \
x

#define file_close(x) fclose(x)

#define file_write(x, y) \
churmant_file = fopen(x, "wb"); \
if not churmant_file then \
  println("(churmant) file not found / invalid path"); \
  longjmp(churmant_buffer, CHURMANT_JUMP); \
end \
fwrite(y, 1, len(y), churmant_file); \
fclose(churmant_file); \
churmant_file = null
#define file_append(x, y) \
churmant_file = fopen(x, "ab"); \
if not churmant_file then \
  println("(churmant) file not found / invalid path"); \
  longjmp(churmant_buffer, CHURMANT_JUMP); \
end \
fwrite(y, 1, len(y), churmant_file); \
fclose(churmant_file); \
churmant_file = null
#define file_find(x) (access(x, F_OK) == success)

#define file_readline(x, y, z) \
fgets(x, y, z); \
x[len(x) - 1] = '\0'

#define assert(x) \
match(x) \
  case(false) \
    printf("assertion failed at line %llu in file '%s'\n", __LINE__, __FILE__); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  close \
end

#define churmant_main \
int main(int argc, string argv[]) do \
  churmant_argc = argc; \
  churmant_argv = argv; \
  churmant_init(); \
  signal(SIGINT, churmant_signal); \
  signal(SIGSEGV, churmant_signal); \
  normal

#define churmant_mend \
  error \
    exit(failure); \
  end \
  exit(success); \
end

#define CHURMANT_VERSION 1.00
#define CHURMANT_MAXFILES 1024
#define CHURMANT_MAXDYNAMICS 8192
#define CHURMANT_UNKNOWN 0
#define CHURMANT_WINDOWS 1
#define CHURMANT_LINUX 2
#define CHURMANT_DARWIN 3
#define CHURMANT_BSD 4
#define CHURMANT_JUMP 42
#define CHURMANT_LINESIZE 1024
#define CHURMANT_ARRAYSIZE 512
#define CHURMANT_ARRAYBUFFERSIZE 512

file churmant_file;
std_ptr churmant_dynamics[CHURMANT_MAXDYNAMICS];
file churmant_files[CHURMANT_MAXFILES];

#if _WIN32
  int churmant_os = CHURMANT_WINDOWS;
#elif __linux
  int churmant_os = CHURMANT_LINUX;
#elif __APPLE__
  int churmant_os = CHURMANT_DARWIN;
#elif __FreeBSD__
  int churmant_os = CHURMANT_BSD;
#else
  int churmant_os = CHURMANT_UNKNOWN;
#endif

int churmant_argc;
string *churmant_argv;
int churmant_findex;
int churmant_dindex;
int churmant_fret;
jmp_buf churmant_buffer;

void churmant_signal(int signum) do
  match(signum)
    case(SIGINT)
      #ifdef churmant_debug
        println("(churmant/debugging) program interrupted");
      #endif

      exit(failure);
    close
    case(SIGSEGV)
      #ifdef churmant_debug
        println("(churmant/debugging) program crashed, recovering");
      #endif

      longjmp(churmant_buffer, CHURMANT_JUMP);
      exit(failure);
    close
  end
end

long len(string var) do
  long i = 0;

  while(var[i])
    i++;
  end

  return i;
end

void swap(long src, long dest) do
  src ^= dest;
  dest ^= src;
  src ^= dest;
end

void split(string_array arr, string var, int delim) do
  string temp = null;
  char original[CHURMANT_LINESIZE] = {};
  int array_i = 0;
  short j = -1;
  bool parsing_string = false;
  allocate(temp, CHURMANT_LINESIZE);

  while(strchr(var, delim))
    j = -1;
    strncpy(temp, var, size(temp));
    
    for(0, temp[i] != delim, 1) do
      j = i;
    end

    if temp[j] != delim then
      j++;
    end
    
    temp[j + 1] = '\0';

    strncpy(original, temp, size(original));

    while(*temp == ' ' or *temp == '\t')
      temp++;
    end

    if strchr(temp, '\"') then
      parsing_string = not parsing_string;
    end

    while(temp[len(temp) - 1] == ' ' or temp[len(temp) - 1] == '\t')
      temp[len(temp) - 1] = '\0';
    end

    var += len(original);

    if *temp == '\0' then
      continue;
    end

    if array_i == CHURMANT_ARRAYSIZE then
      printf("(churmant) too many elements for an array");
      longjmp(churmant_buffer, CHURMANT_JUMP);
    end

    if len(temp) >= CHURMANT_ARRAYBUFFERSIZE then
      printf("(churmant) value 'buffer' size is bigger than the array 'buffer' size");
      longjmp(churmant_buffer, CHURMANT_JUMP);
    end

    strncpy(arr[array_i], temp, CHURMANT_ARRAYBUFFERSIZE);
    array_i++;
  end
  
  strncpy(arr[array_i], var, CHURMANT_ARRAYBUFFERSIZE);
  array_i++;

  parsing_string = false;
end

void churmant_init() do
  for(0, i < CHURMANT_MAXFILES, 1) do
    churmant_files[i] = null;
  end

  for(0, i < CHURMANT_MAXDYNAMICS, 1) do
    churmant_dynamics[i] = null;
  end
end

void churmant_fcheck(file pointer) do
  churmant_fret = -1;

  for(0, i < churmant_findex, 1) do
    if pointer != churmant_files[i] then
      continue;
    end

    churmant_fret = i;
    break;
  end
end

#endif /* __CHURMANT_H */
