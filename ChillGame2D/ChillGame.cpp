#include "ChillGame.h"

void ChillGame::Main() {
    Start();

    InitWindow(screenWidth, screenHeight, "Chill Game");
    SetTargetFPS(60);

    // Initialize player and event
    player.Main();
    event.Main();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
}

void ChillGame::Update() {
    player.Update();
    event.Update();

    // Check for collision with event
    if (event.CheckCollision(player)) {
        event.HandleEvent();
    }
}

void ChillGame::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    player.Draw();
    event.Draw();

    EndDrawing();
}

void ChillGame::Start() {
    // Initialize any game variables here
}
