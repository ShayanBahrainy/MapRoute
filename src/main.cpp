#include "IPV4Path.h"
#include "ASPath.h"
#include "raylib.h"

#include <iostream>

const int WIDTH = 1000;
const int HEIGHT = 800;

std::string typed = "";

int main() {
    InitWindow(WIDTH, HEIGHT, "MapRoute");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(30, HEIGHT/2, 20, GREEN);

        int inputWidth = (typed.size() * 20) > 300 ? typed.size() * 20 : 300;
        DrawRectangle(WIDTH/2 - inputWidth/2, 10, inputWidth, 50, WHITE);

        int c = GetCharPressed();
        if (c >= 32 && c <= 125) {
            typed += (char) c;
        }

        if (IsKeyPressed(KEY_BACKSPACE)) typed = typed.substr(0, typed.size() - 1);

        DrawText(typed.c_str(), WIDTH/2 - inputWidth/2, 10, 60, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}