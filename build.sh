#!/bin/bash
CC="gcc"
CFLAGS="-O3 -ffast-math -march=native -g -I include -Werror=uninitialized"
COMPILER_SOURCE="src/chmc.c"
COMPILER_OUTPUT="bin/chmc"
INSTALLER_SOURCE="src/chminit.c"
INSTALLER_OUTPUT="bin/chminit"
$CC $CFLAGS -o $COMPILER_OUTPUT $COMPILER_SOURCE
$CC $CFLAGS -o $INSTALLER_OUTPUT $INSTALLER_SOURCE
