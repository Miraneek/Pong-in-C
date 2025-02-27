#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define border_sx 0
#define border_sy 0
#define border_ex 1280
#define border_ey 960

int main() {
    struct Squere {
        int x;
        int y;
        int w;
        int h;
        Vector2 vec;
    };

    struct Wall {
        int x;
        int y;
        int w;
        int h;
    };

    struct Wall *wall1 = (struct Wall *) malloc(sizeof(struct Wall));
    wall1->x = 20;
    wall1->y = border_ey / 2;
    wall1->w = 10;
    wall1->h = 100;
    struct Wall *wall2 = (struct Wall *) malloc(sizeof(struct Wall));
    wall2->x = border_ex - 30;
    wall2->y = border_ey / 2;
    wall2->w = 10;
    wall2->h = 100;

    struct Squere *s = (struct Squere *) malloc(sizeof(struct Squere));
    s->x = border_ey / 2;
    s->y = border_ex / 2;
    s->w = 10;
    s->h = 10;
    s->vec = (Vector2){8, -4};

    InitWindow(border_ex, border_ey, "Raylib");
    SetTargetFPS(144);
    char winner = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (winner != 0) {
            const char *game_over_text = "Game Over";
            const int font_size = 120;
            const char *winner_text = winner == 1 ? "Left player won" : "Right player won";
            DrawText(TextFormat("Game Over"), border_ex / 2 - MeasureText(game_over_text, font_size) / 2, border_ey / 3,
                     font_size, RED);
            DrawText(TextFormat(winner_text), border_ex / 2 - MeasureText(winner_text, font_size / 1.5) / 2,
                     border_ey / 2, font_size / 1.5, DARKGREEN);
        } else {
            DrawRectangle(s->x, s->y, s->w, s->h, WHITE);
            DrawRectangle(wall1->x, wall1->y, wall1->w, wall1->h, WHITE);
            DrawRectangle(wall2->x, wall2->y, wall2->w, wall2->h, WHITE);

            s->x += s->vec.x;
            s->y += s->vec.y;

            if (s->y > border_ey - s->w) {
                s->vec.y *= -1;
            } else if (s->y < border_sy) {
                s->vec.y *= -1;
            }

            if ((s->y >= wall1->y && s->y <= wall1->y + wall1->h) && (s->x <= wall1->x + wall1->w)) {
                s->vec.x *= -1 * 1.1;
            } else if ((s->y >= wall2->y && s->y <= wall2->y + wall2->h) && (s->x >= wall2->x - s->w)) {
                s->vec.x *= -1 * 1.1;
            }

            if (s->x > border_ex - s->h) {
                winner = 1;
            } else if (s->x < border_sx) {
                winner = 2;
            }


            if (IsKeyDown(KEY_UP) && wall2->y > 20) {
                wall2->y -= 15;
            }
            if (IsKeyDown(KEY_DOWN) && wall2->y < (border_ey - 20) - wall2->h) {
                wall2->y += 15;
            }

            if (IsKeyDown(KEY_W) && wall1->y > 20) {
                wall1->y -= 15;
            }
            if (IsKeyDown(KEY_S) && wall1->y < (border_ey - 20) - wall1->h) {
                wall1->y += 15;
            }
        }
        EndDrawing();
    }

    CloseWindow();
    free(s);
    free(wall1);
    free(wall2);
    return 0;
}
