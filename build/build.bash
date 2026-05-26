#!/bin/bash
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

cc \
  "$ROOT/src/main.c" \
  "$ROOT/src/paddle.c" \
  "$ROOT/src/ball.c" \
  "$ROOT/src/ui.c" \
  "$ROOT/src/particles.c" \
  -I"$ROOT/include" \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
  -o "$ROOT/build/game"