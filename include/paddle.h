#ifndef PADDLE_H
#define PADDLE_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    Rectangle bounds;
    float speed;
    Color color;
    bool isAI;
} Paddle;

void PaddleInit(Paddle *p, float x, float y, Color color, bool isAI);
void PaddleUpdate(Paddle *p, float ballY, int upKey, int downKey);
void PaddleDraw(Paddle *p, Texture2D tex);

#endif