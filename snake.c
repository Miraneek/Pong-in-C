#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 1250
#define HEIGHT 1250

#define CELL_SIZE 25
#define CELL_COUNT 50
#define MAX_SNAKE_LENGTH 100
bool game_over = false;

struct Snake {
    Vector2 positions[MAX_SNAKE_LENGTH];
    Color color;
    int length;
    int direction;

    void DrawSnake() const {
        for (int i = 0; i < length; i++) {
            DrawRectangle((positions[i].x) * CELL_SIZE, positions[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, WHITE);
        }
    }

    void UpdateSnake() {
        for (int i = length - 1; i > 0; i--) {
            positions[i] = positions[i - 1];
        }

        switch (direction) {
            case 0: // UP
                positions[0].y -= 1;
                break;
            case 1: // DOWN
                positions[0].y += 1;
                break;
            case 2: // LEFT
                positions[0].x -= 1;
                break;
            case 3: // RIGHT
                positions[0].x += 1;
                break;
            default:
                break;
        }


        for (int i = 1; i < length; i++) {
            if (positions[i].x == positions[0].x && positions[i].y == positions[0].y) {
                game_over = true;
            }
        }

        // Wrap around the screen
        if ((positions[0].x < 0) || (positions[0].x >= CELL_COUNT) || (positions[0].y < 0) || (
                positions[0].y >= CELL_COUNT))
            game_over = true;
    }

    void GrowSnake() {
        if (length < MAX_SNAKE_LENGTH) {
            positions[length] = positions[length - 1];
            length++;
        }
    }
};

struct Food {
    Vector2 position;

    void DrawFood() const {
        DrawRectangle(position.x * CELL_SIZE, position.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
    }

    void GenerateFood(struct Snake *s) {
        bool collides_with_snake = false;
        position.x = GetRandomValue(0, CELL_COUNT - 1);
        position.y = GetRandomValue(0, CELL_COUNT - 1);

        for (int i = 0; i < s->length; i++) {
            if (position.x == s->positions[i].x && position.y == s->positions[i].y) {
                collides_with_snake = true;
            }

            if (collides_with_snake) {
                GenerateFood(s);
            }
        }
    }
};

int main() {
    InitWindow(WIDTH, HEIGHT, "Snake");
    SetTargetFPS(8); // Slower framerate for easier gameplay

    srand(time(nullptr));

    struct Food food;
    struct Snake snake = {{12, 12}, WHITE, 20, 3};
    food.GenerateFood(&snake);
    int score = 0;

    while (!game_over) {
        // Handle input
        if (IsKeyDown(KEY_UP) && snake.direction != 1) snake.direction = 0;
        if (IsKeyDown(KEY_DOWN) && snake.direction != 0) snake.direction = 1;
        if (IsKeyDown(KEY_LEFT) && snake.direction != 3) snake.direction = 2;
        if (IsKeyDown(KEY_RIGHT) && snake.direction != 2) snake.direction = 3;

        // Update game state
        snake.UpdateSnake();

        // Check for collision with food
        if (snake.positions[0].x == food.position.x && snake.positions[0].y == food.position.y) {
            snake.GrowSnake();
            food.GenerateFood(&snake);
            score++;
        }

        BeginDrawing();
        ClearBackground(BACKGROUND);

        food.DrawFood();
        snake.DrawSnake();
        DrawText(TextFormat("Score: %d", score),(WIDTH - MeasureText(TextFormat("Score: %d", score), 30 ) - 20),  20, 30, LIGHTGRAY);

        EndDrawing();
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND);
        DrawText("Game Over", (WIDTH / 2 - MeasureText("Game Over", 80) / 2), HEIGHT / 2 - 70, 80, RED);
        DrawText(TextFormat("Score: %d", score), (WIDTH / 2 - MeasureText(TextFormat("Score: %d", score), 60) / 2),
                 HEIGHT / 2 + 10, 60, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
