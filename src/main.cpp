#include <chrono>
#include <future>
#include <iostream>

#include "IPV4Path.h"
#include "ASPath.h"
#include "raylib.h"

const int WIDTH = 1000;
const int HEIGHT = 800;


std::future<IPV4Path*> futureIP;
std::future<ASPath*> futureAS;

IPV4Path *ipPath;
ASPath *asPath;

std::string typed = "";

enum ProgramState {ACCEPT_INPUT, WAITIP, WAITAS};

ProgramState state = ACCEPT_INPUT;

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "MapRoute");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(60, GetScreenHeight()/2, 50, GREEN);

        int inputWidth = MeasureText(typed.c_str(), 60) + 20 > 300 ? MeasureText(typed.c_str(), 60) + 20 : 300;
        DrawRectangle(GetScreenWidth()/2 - inputWidth/2, 10, inputWidth, 70, WHITE);

        int c = GetCharPressed();
        if (c >= 32 && c <= 125 && state == ACCEPT_INPUT){
            typed += (char) c;
        }

        if (IsKeyPressed(KEY_BACKSPACE) && state == ACCEPT_INPUT) typed = typed.substr(0, typed.size() - 1);
        if (IsKeyPressed(KEY_ENTER) && state == ACCEPT_INPUT) {
            std::cout << typed << std::endl;
            futureIP = std::async(std::launch::async, IPV4Path::fromDomain, typed);
            state = WAITIP;
        }

        DrawText(typed.c_str(), GetScreenWidth()/2 - inputWidth/2 + 10, 10, 60, GREEN);

        if (state == WAITIP && futureIP.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            delete ipPath;
            ipPath = futureIP.get();

            futureAS = std::async(std::launch::async, ASPath::fromIpPath, *ipPath);

            state = WAITAS;
        }

        if (state == WAITAS && futureAS.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {

            delete asPath;
            asPath = futureAS.get();

            state = ACCEPT_INPUT;
        }

        if (ipPath != nullptr) {
            for (int i = 0; i < ipPath->size(); i++) {
                const int verticalVariance = 50;
                const int X = (250 * (i + 1)) + 60;
                const int Y = verticalVariance * (i % 2 == 1 ? 1 : -1) + GetScreenHeight()/2;

                if (CheckCollisionPointCircle({static_cast<float>(X), static_cast<float>(Y)}, {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}, 50)) {
                    std::string text = ipPath->at(i).toString();
                    int textWidth = MeasureText(text.c_str(), 50);
                    DrawText(text.c_str(), X - textWidth/2, Y - 200, 50, WHITE);
                }

                DrawCircle(X, Y, 50, GREEN);
            }
        }


        EndDrawing();
    }

    delete asPath;
    delete ipPath;

    CloseWindow();

    return 0;
}