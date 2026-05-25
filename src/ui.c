#include "../include/ui.h"

static void DrawDigit(Texture2D digits, int digit, int x, int y) {
    Rectangle src = { digit * 32.0f, 0, 32, 48 };
    DrawTextureRec(digits, src, (Vector2){ x, y }, WHITE);
}

void UiDrawNet(Texture2D netSeg) {
    int screenH = 560;
    int segH    = netSeg.height;
    for (int y = 0; y < screenH; y += segH + 8) {
        DrawTexture(netSeg, 400 - 2, y, WHITE);
    }
}

void UiDrawScore(Texture2D digitsLeft, Texture2D digitsRight, int scoreL, int scoreR) {
    DrawDigit(digitsLeft, scoreL, 800/2 - 80, 20);
    DrawDigit(digitsRight, scoreR, 800/2 + 48, 20);
}

void UiDrawMenu(Texture2D panel, Texture2D btnNormal, Texture2D btnHover,bool hovered1, bool hovered2, float btnx, float btny, float btnScale) {
    Color neonCyan = GetColor(0x00ffdcff);
    Color neonPink = GetColor(0xff00b4ff);

    DrawTextureEx(panel, (Vector2){ 0, 0 }, 0.0f, 2.0f, WHITE);

    if (hovered1) {
        DrawTextureEx(btnHover, (Vector2){ btnx, btny }, 0.0f, btnScale, WHITE);
        DrawTextEx(GetFontDefault(), "PLAY",
                   (Vector2){ 800/2 - 20, btny + 10 }, 20, 1, neonPink);
    } else {
        DrawTextureEx(btnNormal, (Vector2){ btnx, btny }, 0.0f, btnScale, WHITE);
        DrawTextEx(GetFontDefault(), "PLAY",
                   (Vector2){ 800/2 - 20, btny + 10 }, 20, 1, neonCyan);
    }
    if (hovered2) {
        DrawTextureEx(btnHover, (Vector2){ btnx, btny + btnNormal.height*btnScale }, 0.0f, btnScale, WHITE);
        DrawTextEx(GetFontDefault(), "PLAY VS AI",
                   (Vector2){ 800/2 - 60, btny + 10 + btnNormal.height*btnScale }, 20, 1, neonPink);
    } else {
        DrawTextureEx(btnNormal, (Vector2){ btnx, btny + btnNormal.height*btnScale }, 0.0f, btnScale, WHITE);
        DrawTextEx(GetFontDefault(), "PLAY VS AI",
                   (Vector2){ 800/2 - 60, btny + 10 + btnNormal.height*btnScale }, 20, 1, neonCyan);
    }
}

void UiDrawGameOver(int scoreL, int scoreR) {
    Color neonCyan = GetColor(0x00ffdcff);
    Color neonPink = GetColor(0xff00b4ff);

    const char *msg    = scoreL > scoreR ? "PLAYER 1 WINS" : "PLAYER 2 WINS";
    Color        color = scoreL > scoreR ? neonCyan : neonPink;

    DrawText(msg, 800/2 - MeasureText(msg, 30)/2, 220, 30, color);
    DrawText("PRESS R TO RESTART", 800/2 - MeasureText("PRESS R TO RESTART", 16)/2,
             270, 16, GRAY);
}