#pragma once
#include "raylib.h"
#include "Player.h"
#include "Event.h"

class ChillGame {
public:
    void Main();
    void Start();
    void Update();
    void Draw();

    const int screenWidth = 800;
    const int screenHeight = 450;

private:
    Player player;
    Event event;
};
