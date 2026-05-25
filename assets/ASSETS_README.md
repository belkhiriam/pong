# Pong Assets — Raylib Integration Guide

## Sprites (PNG, RGBA)

| File | Size | Usage |
|------|------|-------|
| `ball.png` | 64×64 | Ball sprite (scale to 16×16 in game) |
| `ball_16.png` | 16×16 | Ball sprite at native res |
| `paddle_left.png` | 48×256 | Left paddle — cyan neon |
| `paddle_right.png` | 48×256 | Right paddle — pink neon |
| `net_segment.png` | 4×20 | Tile vertically for centre line |
| `digits.png` | 320×48 | 7-segment sheet — 10 digits × 32 px wide |
| `ui_panel.png` | 400×280 | Menu/overlay background panel |
| `button_play.png` | 200×56 | Button normal state |
| `button_play_hover.png` | 200×56 | Button hover state |
| `particle.png` | 8×8 | Spark for hit effects |

## Audio (WAV, 44 100 Hz, Mono, 16-bit)

| File | Trigger |
|------|---------|
| `sfx_paddle_hit.wav` | Ball bounces off a paddle |
| `sfx_wall_hit.wav` | Ball bounces off top/bottom wall |
| `sfx_score.wav` | A player scores a point |
| `sfx_start.wav` | Match starts / countdown ends |
| `sfx_game_over.wav` | Game ends |
| `sfx_menu_blip.wav` | Menu cursor move / button press |
| `music_loop.wav` | Background music — loop seamlessly |

---

## Raylib Loading Snippets

```c
// --- Textures ---
Texture2D ball        = LoadTexture("assets/ball.png");
Texture2D paddleLeft  = LoadTexture("assets/paddle_left.png");
Texture2D paddleRight = LoadTexture("assets/paddle_right.png");
Texture2D netSeg      = LoadTexture("assets/net_segment.png");
Texture2D digits      = LoadTexture("assets/digits.png");   // sprite sheet
Texture2D panel       = LoadTexture("assets/ui_panel.png");
Texture2D btnNormal   = LoadTexture("assets/button_play.png");
Texture2D btnHover    = LoadTexture("assets/button_play_hover.png");
Texture2D particle    = LoadTexture("assets/particle.png");

// --- Audio ---
InitAudioDevice();
Sound sfxPaddle   = LoadSound("assets/sfx_paddle_hit.wav");
Sound sfxWall     = LoadSound("assets/sfx_wall_hit.wav");
Sound sfxScore    = LoadSound("assets/sfx_score.wav");
Sound sfxStart    = LoadSound("assets/sfx_start.wav");
Sound sfxGameOver = LoadSound("assets/sfx_game_over.wav");
Sound sfxBlip     = LoadSound("assets/sfx_menu_blip.wav");
Music bgMusic     = LoadMusicStream("assets/music_loop.wav");
bgMusic.looping   = true;
PlayMusicStream(bgMusic);
```

## Drawing the Ball

```c
// Scale the 64x64 texture down to your game size
float ballRadius = 8.0f;
DrawTextureEx(ball, (Vector2){ball_x - ballRadius, ball_y - ballRadius},
              0.0f, (2.0f * ballRadius) / ball.width, WHITE);
```

## Drawing a Digit from the Sheet

```c
// digits.png: each digit is 32px wide, 48px tall
void DrawDigit(Texture2D sheet, int digit, int x, int y) {
    Rectangle src = { digit * 32.0f, 0, 32, 48 };
    DrawTextureRec(sheet, src, (Vector2){x, y}, WHITE);
}
// Example: draw score "3" at screen position (100, 20)
DrawDigit(digits, 3, 100, 20);
```

## Drawing the Net

```c
// Tile net_segment.png down the centre line
int screenH = GetScreenHeight();
int segH    = netSeg.height;           // 20px
for (int y = 0; y < screenH; y += segH + 8) {
    DrawTexture(netSeg, GetScreenWidth()/2 - 2, y, WHITE);
}
```

## Music Update (call every frame)

```c
UpdateMusicStream(bgMusic);   // must be called every frame inside BeginDrawing/EndDrawing
```

## Unloading

```c
UnloadTexture(ball); // repeat for each texture
UnloadSound(sfxPaddle); // repeat for each sound
UnloadMusicStream(bgMusic);
CloseAudioDevice();
```
