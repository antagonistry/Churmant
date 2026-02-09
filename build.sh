#!/bin/bash
CC="gcc"
ECHO="echo"
CFLAGS="-O3 -Os -ffunction-sections -fdata-sections -static -g --include=include/churmant.h -Werror=uninitialized -Werror=return-local-addr -std=c99"
LIBRARIES=""
COMPILER_SOURCE="src/chmc.c"
COMPILER_OUTPUT="bin/chmc"
INSTALLER_SOURCE="src/chminit.c"
INSTALLER_OUTPUT="bin/chminit"
$ECHO "(churmant/build) compiling source file '$COMPILER_SOURCE'"
$CC $CFLAGS -o $COMPILER_OUTPUT $COMPILER_SOURCE $LIBRARIES
ret=$?

if [[ ret -eq 0 ]]; then
  $ECHO "(churmant/build) compiled, binary at '$COMPILER_OUTPUT'"
elif [[ ret -eq 127 ]]; then
  $ECHO "(churmant/build) 'gcc' is not installed"
else
  $ECHO "(churmant/build) failed to compile source file '$COMPILER_SOURCE'"
fi

$ECHO "(churmant/build) compiling source file '$COMPILER_SOURCE'"
$CC $CFLAGS -o $INSTALLER_OUTPUT $INSTALLER_SOURCE $LIBRARIES
ret=$?

if [[ ret -eq 0 ]]; then
  $ECHO "(churmant/build) compiled, binary at '$INSTALLER_OUTPUT'"
elif [[ ret -eq 127 ]]; then
  $ECHO "(churmant/build) 'gcc' is not installed"
else
  $ECHO "(churmant/build) failed to compile source file '$INSTALLER_SOURCE'"
fi
