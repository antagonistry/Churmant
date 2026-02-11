#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdalign.h>
#include <stdarg.h>
#include <pthread.h>

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
#define bool _Bool
#define byte _Bool
#define char char
#define inline inline
#define align alignas(32)
#define ptr void*
#define tuple ptr*
#define file FILE*
#define true 1
#define false 0
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
#define return(x) return(x);
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
    if not churmant_dynamics[i] then \
      continue; \
    end \
    free(churmant_dynamics[i]); \
  end \
  exit(x)

#define union(x) union x {
#define struct(x) struct x {
#define enum(x) enum x {

#ifndef churmant_malloc
  #define allocate(x, y) \
  if x != null then \
    println("(churmant) allocated pointer"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  x = malloc(y); \
  churmant_dynamics[churmant_dindex] = x; \
  churmant_dindex++; \
  if churmant_dindex > CHURMANT_MAXDYNAMICS then \
    println("(churmant) dynamics overflows"); \
    exit(failure); \
  end \
  x
#else
  #define allocate(x, y) x = malloc(y)
#endif

#define size(x) sizeof(x)
#define type(x) typeof(x)

#ifndef churmant_malloc
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
  #define file_ropen(x, y) \
  if x then \
    println("(churmant) allocated pointer"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  x = fopen(y, "rb"); \
  if not x then \
    println("(churmant) file not found / invalid path"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  churmant_files[churmant_findex] = x; \
  churmant_findex++; \
  if churmant_findex > CHURMANT_MAXFILES then \
    println("(churmant) files overflows"); \
    exit(failure); \
  end \
  x
  #define file_wopen(x, y) \
  if x then \
    println("(churmant) allocated pointer"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  x = fopen(y, "wb"); \
  if not x then \
    println("(churmant) file not found / invalid path"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  churmant_files[churmant_findex] = x; \
  churmant_findex++; \
  if churmant_findex > CHURMANT_MAXFILES then \
    println("(churmant) files overflows"); \
    exit(failure); \
  end \
  x
  #define file_close(x) \
  if not x then \
    println("(churmant) unallocated pointer"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  churmant_fcheck(x); \
  if churmant_fret == -1 then \
    println("(churmant) file not found"); \
    longjmp(churmant_buffer, CHURMANT_JUMP); \
  end \
  fclose(x); \
  x
#else
  #define file_open(x, y) x = fopen(y, "r+")
  #define file_ropen(x, y) x = fopen(y, "rb")
  #define file_wopen(x, y) x = fopen(y, "wb")
  #define file_close(x, y) fclose(x)
#endif

#define file_find(x) (access(x, F_OK) == success)

#define file_readline(x, y, z) \
fgets(x, y, z); \
x[strlen(x) - 1] = '\0'

#define assert(x) \
match(x) \
  case(false) \
    printf("assertion failed at line %llu in file '%s'\n", __LINE__, __FILE__); \
    exit(failure); \
  close \
end

#define churmant_main \
int main(int argc, string argv[]) do \
  churmant_argc = argc; \
  churmant_argv = argv; \
  signal(SIGINT, churmant_signal); \
  signal(SIGSEGV, churmant_signal); \
  normal

#define churmant_mend \
  error \
    exit(failure); \
  end \
  exit(success); \
end

#define CHURMANT_MAXFILES 256
#define CHURMANT_MAXDYNAMICS 512
#define CHURMANT_UNKNOWN 0
#define CHURMANT_WINDOWS 1
#define CHURMANT_LINUX 2
#define CHURMANT_DARWIN 3
#define CHURMANT_BSD 4
#define CHURMANT_JUMP 42

ptr churmant_dynamics[CHURMANT_MAXDYNAMICS];
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

func(churmant_fcheck(file pointer))
  churmant_fret = -1;

  for(0, i < CHURMANT_MAXFILES, 1) do
    if pointer != churmant_files[i] then
      continue;
    end

    churmant_fret = i;
    break;
  end
fend(abort)

#endif /* __CHURMANT_H */
