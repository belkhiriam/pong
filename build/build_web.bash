#!/bin/bash
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
RAYLIB_SRC="/home/amar/raylib/src"   

source /home/amar/emsdk/emsdk_env.sh  

emcc \
  "$ROOT/src/web_main.c" \
  "$ROOT/src/paddle.c" \
  "$ROOT/src/ball.c" \
  "$ROOT/src/ui.c" \
  "$ROOT/src/particles.c" \
  -I"$ROOT/include" \
  -I"$RAYLIB_SRC" \
  "$RAYLIB_SRC/libraylib.web.a" \
  -o "$ROOT/build/web/index.html" \
  --shell-file "$ROOT/build/shell.html" \
  --preload-file "$ROOT/assets@assets" \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s TOTAL_MEMORY=67108864 \
  -DPLATFORM_WEB \
  -Os