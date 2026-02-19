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

enum ViewMode {IP_PATH, AS_PATH};

ViewMode view_mode = IP_PATH;

int loadingIconCounter = 0;

enum LoadingIconState {ONE, TWO, THREE};
LoadingIconState loadingState;

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "MapRoute");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(60, 60, 50, GREEN);

        int inputWidth = MeasureText(typed.c_str(), 60) + 20 > 300 ? MeasureText(typed.c_str(), 60) + 20 : 300;
        DrawRectangle(GetScreenWidth()/2 - inputWidth/2, 10, inputWidth, 70, WHITE);

        if (state != ACCEPT_INPUT) {
            if (loadingIconCounter % 60 == 0) {
                if (loadingState == ONE) loadingState = TWO;
                else if (loadingState == TWO) loadingState = THREE;
                else if (loadingState == THREE) loadingState = ONE;
            }

            switch(loadingState) {
                case ONE:
                    DrawText(".", GetScreenWidth()/2, 50, 70, GREEN);
                    DrawText(".", GetScreenWidth()/2 + 10, 50, 60, GREEN);
                    DrawText(".", GetScreenWidth()/2 + 20, 50, 60, GREEN);
                    break;
                case TWO:
                    DrawText(".", GetScreenWidth()/2, 50, 60, GREEN);
                    DrawText(".", GetScreenWidth()/2 + 10, 50, 70, GREEN);
                    DrawText(".", GetScreenWidth()/2 + 20, 50, 60, GREEN);
                    break;
                case THREE:
                    DrawText(".", GetScreenWidth()/2, 50, 60, GREEN);
                    DrawText(".", GetScreenWidth()/2 + 10, 50, 60, GREEN);
                    DrawText(".", GetScreenWidth()/2 + 20, 50, 70, GREEN);
                    break;
            }

            loadingIconCounter++;
        }


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

        if (ipPath != nullptr && view_mode == IP_PATH) {
            const float STEP_X = (GetScreenWidth() - 50) / (float) (ipPath->size() + 1);
            const float STEP_Y = (GetScreenHeight() - 50) / (float) (ipPath->size() + 1);
            float prev_x = 60;
            float prev_y = 60;

            for (int i = 0; i < ipPath->size(); i++) {
                const int VERTICAL_VARIANCE = 50;
                const int X = (STEP_X * (i + 1)) + 60;
                const int Y = (STEP_Y * (i + 1)) + 60 + (VERTICAL_VARIANCE * (i % 2 == 1 ? 1 : -1));

                if (CheckCollisionPointCircle({static_cast<float>(X), static_cast<float>(Y)}, {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}, 50)) {
                    std::string text = ipPath->at(i).toString();
                    int textWidth = MeasureText(text.c_str(), 50);
                    DrawText(text.c_str(), X - textWidth/2 < GetScreenWidth() ? X - textWidth/2 : X - textWidth, Y - 200 > 0 ? Y - 200 : Y + 200, 50, WHITE);
                }

                DrawLine(prev_x, prev_y, X, Y, WHITE);
                DrawCircle(X, Y, 50, GREEN);

                prev_x = X;
                prev_y = Y;
            }
        }

        if (asPath != nullptr && view_mode == AS_PATH) {
            const float STEP_X = (GetScreenWidth() - 50) / (float) (asPath->size() + 1);
            const float STEP_Y = (GetScreenHeight() - 50) / (float) (asPath->size() + 1);
            float prev_x = 60;
            float prev_y = 60;

            for (int i = 0; i < asPath->size(); i++) {
                const int VERTICAL_VARIANCE = 50;
                const int X = (STEP_X * (i + 1)) + 60;
                const int Y = (STEP_Y * (i + 1)) + 60 + (VERTICAL_VARIANCE * (i % 2 == 1 ? 1 : -1));

                if (CheckCollisionPointCircle({static_cast<float>(X), static_cast<float>(Y)}, {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}, 50)) {
                    std::string topText = std::to_string(asPath->at(i).getASN());
                    int topTextWidth = MeasureText(topText.c_str(), 50);
                    DrawText(topText.c_str(), X - topTextWidth/2 < GetScreenWidth() ? X - topTextWidth/2 : X - topTextWidth, Y - 200 > 0 ? Y - 200 : Y + 200, 50, WHITE);

                    std::string bottomText = asPath->at(i).getName();
                    int bottomTextWidth = MeasureText(bottomText.c_str(), 50);
                    DrawText(bottomText.c_str(), X - bottomTextWidth/2 < GetScreenWidth() ? X - bottomTextWidth/2 : X - bottomTextWidth, Y + 300 < GetScreenHeight() - 50 ? Y + 300 : Y - 300, 50, WHITE);
                }

                DrawLine(prev_x, prev_y, X, Y, WHITE);
                DrawCircle(X, Y, 50, GREEN);

                prev_x = X;
                prev_y = Y;
            }
        }

        const int X = GetScreenWidth() - 10 - 100;

        Rectangle ipButton = {static_cast<float>(X), 10, 100, 60};
        Rectangle asButton = {static_cast<float>(X - 100), 10, 100, 60};

        DrawRectangleRec(ipButton, view_mode == IP_PATH ? GREEN : WHITE);
        DrawText("IP", ipButton.x + 10, ipButton.y, 60, view_mode == IP_PATH ? WHITE : GREEN);
        DrawRectangleRec(asButton, view_mode == AS_PATH ? GREEN : WHITE);
        DrawText("AS", asButton.x + 10, asButton.y, 60, view_mode == AS_PATH ? WHITE : GREEN);

        if (CheckCollisionPointRec(Vector2{static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}, ipButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            view_mode = IP_PATH;
        }

        if (CheckCollisionPointRec(Vector2{static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}, asButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            view_mode = AS_PATH;
        }

        EndDrawing();
    }

    delete asPath;
    delete ipPath;

    CloseWindow();

    return 0;
}