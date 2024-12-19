#pragma once
#include "raylib.h"
#include "Player.h"

class Event {
public:
    Event() :
        eventSprite{ 0 },
        starSprite{ 0 },
        starGraySprite{ 0 },
        isCollected(false),
        isTriggered(false),
        eventPosition{ 0, 0 },
        eventType(1)
    {
    }

    ~Event() {
        UnloadTexture(eventSprite);
        UnloadTexture(starSprite);
        UnloadTexture(starGraySprite);
    }

    void Start();
    void Update();
    void HandleEvent();
    void Main(int eventType, Vector2 position);
    void Draw();
    bool CheckCollision(const Player& player);
    void SetCollected(bool collected) { isCollected = collected; }
    bool IsTriggered() const { return isTriggered; }
    void SetTriggered(bool triggered) { isTriggered = triggered; }
    int GetType() const { return eventType; }
    Vector2 GetPosition() const { return eventPosition; }

private:
    int eventType;
    Vector2 eventPosition;
    Texture2D eventSprite;
    Texture2D starSprite;
    Texture2D starGraySprite;
    bool isCollected;
    bool isTriggered;
};
