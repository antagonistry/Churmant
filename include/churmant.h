#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifndef __CHURMANT_H
#define __CHURMANT_H
#define integer int
#define string char*
#define float double
#define bool _Bool
#define byte _Bool
#define character char
#define function void*
#define true 1
#define false 0
#define successful 0
#define failure 1
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
#define for(x, y, z) for(int_fast64_t i = x; y; i += z)
#define constant(x) #x
#define new(x) malloc(x)
#define delete(x) free(x)
#endif /* __CHURMANT_H */
