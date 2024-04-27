#!/bin/bash

set -xe

which clang
clang -Os -fno-builtin -Wall -Wextra -Wswitch-enum --target=wasm32 --no-standard-libraries -Wl,--export=foo -Wl,--export=app_init -Wl,--export=cell -Wl,--export=getValue -Wl,--no-entry -Wl,--allow-undefined  -o app.wasm app.c