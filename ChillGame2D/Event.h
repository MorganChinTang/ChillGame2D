#pragma once
#include "raylib.h"
#include "Player.h"

class Event {
public:
    void Main();
    void Start();
    void Update();
    void Draw();

    bool CheckCollision(const Player& player);
    void HandleEvent();

private:
    Texture2D eventSprite;
    Rectangle eventRect;
    Vector2 position;
    bool isTriggered;
};
