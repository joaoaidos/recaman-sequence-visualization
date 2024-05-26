#! /bin/env bash

if [ ! -f ./raylib/src/libraylib.a ]; then
  make -C ./raylib/src/
fi
gcc -o main main.c -I./raylib/src -L./raylib/src -lraylib -lm -ldl -lpthread

