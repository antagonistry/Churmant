#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>
#ifndef __CHURMANT_H
#define __CHURMANT_H
#define int int
#define long int_fast64_t
#define string char*
#define float double
#define bool _Bool
#define byte _Bool
#define char char
#define func intptr_t
#define ptr intptr_t
#define tuple ptr*
#define true 1
#define false 0
#define success 0
#define failure 1
#define not !
#define or ||
#define and &&
#define if if (
#define then ) {
#define end }
#define do {
#define else } else {
#define elif } else if (
#define match(x) switch((int_fast64_t) x) {
#define case(x) case(x): {
#define close } break;
#define default default: {
#define print(x) fputs(x, stdout)
#define println(x) puts(x)
#define while(x) while(x)
#define for(x, y, z) for(int_fast64_t i = x; y; i += z)
#define const(x) #x
#define new(x) malloc(x)
#define delete(x) free(x)
#define size(x) sizeof(x)
#define type(x) typeof(x)
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
  signal(SIGSEGV, churmant_signal);
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
