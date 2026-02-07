@echo off
set CC=gcc
set CFLAGS=-O3 -ffast-math -march=native -g -I include -Werror=uninitialized
set COMPILER_SOURCE=src/chmc.c
set COMPILER_OUTPUT=bin/chmc
set INSTALLER_SOURCE=src/chminit.c
set INSTALLER_OUTPUT=bin/chminit
%CC% %CFLAGS% -o %COMPILER_OUTPUT% %COMPILER_SOURCE%
%CC% %CFLAGS% -o %INSTALLER_OUTPUT% %INSTALLER_SOURCE%
