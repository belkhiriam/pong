#ifndef BALL_H
#define BALL_H

#include <raylib.h>
#include "paddle.h"

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float radius;
} Ball;

void BallInit(Ball *b);
int  BallUpdate(Ball *b, Paddle *left, Paddle *right, Sound wallHitSound, Sound paddleHitSound); // returns 1 = right scores, -1 = left scores, 0 = nothing
void BallDraw(Ball *b, Texture2D tex);

#endif