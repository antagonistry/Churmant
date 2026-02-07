#!/bin/bash
CC="gcc"
CFLAGS="-O3 -ffast-math -march=native -g -I include"
SOURCE="src/chmc.c"
OUTPUT="bin/chmc"
$CC $CFLAGS -o $OUTPUT $SOURCE
