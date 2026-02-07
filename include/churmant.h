#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#ifndef __CHURMANT_H
#define __CHURMANT_H
#define int int_fast32_t
#define long int_fast64_t
#define string char*
#define float double
#define bool _Bool
#define byte _Bool
#define char char
#define func intptr_t
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
#define elif } else if (
#define match(x) switch((intptr_t) x) {
#define case(x) case(x): {
#define close } break;
#define default default: {
#define print(x) fputs(x, stdout)
#define println(x) puts(x)
#define while(x) while(x) {
#define for(x, y, z) for(long i = x; y; i += z)
#define const(x) #x
#define final const
#define quit(x) \
  for(0, i < churmant_findex, 1) do \
    fclose(churmant_files[i]); \
  end \
  for(0, i < churmant_dindex, 1) do \
    free(churmant_dynamics[i]); \
  end \
  free(churmant_files); \
  free(churmant_dynamics); \
  exit(x)
#define union(x) union x {
#define struct(x) struct x {
#define enum(x) enum {
#ifndef churmant_calloc
  #define allocate(x, y) \
  x = malloc(y); \
  churmant_dynamics[churmant_dindex] = x; \
  churmant_dindex++; \
  if churmant_dindex > CHURMANT_MAXDYNAMICS then \
    printf("\n\n[churmant dynamics overflows]\n"); \
    exit(failure); \
  end \
  x
#else
  #define allocate(x, y) x = malloc(y)
#endif
#define size(x) sizeof(x)
#define type(x) typeof(x)
#ifndef churmant_calloc
  #define file_open(x, y) \
  x = fopen(y, "r+"); \
  churmant_files[churmant_findex] = x; \
  churmant_findex++; \
  if churmant_findex > CHURMANT_MAXFILES then \
    printf("\n\n[churmant files overflows]\n"); \
    exit(failure); \
  end \
  x
#else
  #define file_open(x, y) x = fopen(y, "r+")
#endif
#define file_find(x) (access(x, F_OK) == success)
#define file_readline(x, y, z) \
fgets(x, y, z); \
x[strlen(x) - 1] = '\0'
#define assert(x) \
match(x) \
  case(false) \
    printf("\n\n[assertion failed at line %llu in file '%s']\n", __LINE__, __FILE__); \
    exit(failure); \
  close \
end
#define churmant_main \
int main(int argc, string argv[]) do \
  signal(SIGINT, churmant_signal); \
  signal(SIGSEGV, churmant_signal); \
  churmant_files = malloc(sizeof(FILE) * CHURMANT_MAXFILES); \
  churmant_dynamics = malloc(sizeof(intptr_t) * CHURMANT_MAXDYNAMICS);
#define churmant_mend \
  for(0, i < churmant_findex, 1) do \
    fclose(churmant_files[i]); \
  end \
  for(0, i < churmant_dindex, 1) do \
    free(churmant_dynamics[i]); \
  end \
  free(churmant_files); \
  free(churmant_dynamics); \
  return(success); \
end
final int CHURMANT_MAXFILES = 256;
final int CHURMANT_MAXDYNAMICS = 512;
ptr *churmant_dynamics;
file *churmant_files;
int churmant_findex;
int churmant_dindex;
void churmant_signal(int signum) do
  match(signum)
    case(SIGINT)
      printf("\n\n[program interrupted]\n");
      exit(failure);
    close
    case(SIGSEGV)
      printf("\n\n[program crashed]\n");
      exit(failure);
    close
  end
end
#endif /* __CHURMANT_H */
