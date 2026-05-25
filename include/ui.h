#ifndef UI_H
#define UI_H

#include <raylib.h>

void UiDrawMenu(Texture2D panel, Texture2D btnNormal, Texture2D btnHover,bool hovered1, bool hovered2, float btnx, float btny, float btnScale);
void UiDrawNet(Texture2D netSeg);
void UiDrawScore(Texture2D digitsLeft, Texture2D digitsRight, int scoreL, int scoreR);
void UiDrawGameOver(int scoreL, int scoreR);

#endif