#pragma once
#include "raylib.h"
#include "Player.h"

enum class EventType {
    EVENT_1
};

class Event {
public:
    Event() :
        eventSprite{ 0 },
        starGraySprite{ 0 },
        starSprite{ 0 },
        eventRect{ 0, 0, 0, 0 },
        position{ 0, 0 },
        starPosition{ 0, 0 },
        isTriggered(false),
        isCollected(false),
        eventType(EventType::EVENT_1)
    {
    }

    ~Event() {
        UnloadTexture(eventSprite);
        UnloadTexture(starGraySprite);
        UnloadTexture(starSprite);
    }

    void Main(EventType type, Vector2 pos);
    void Initialize(Vector2 pos);
    void Start();
    void Update();
    void Draw();
    bool CheckCollision(Player& player);
    bool IsTriggered() const { return isTriggered; }
    bool IsCollected() const { return isCollected; }
    void SetCollected(bool collected) { isCollected = collected; }

private:
    void HandleEvent();

    static constexpr float EVENT_SCALE = 3.0f;  // 3 times the original 0.6 scale
    static constexpr float STAR_SCALE = 0.3f;

    Texture2D eventSprite;
    Texture2D starGraySprite;
    Texture2D starSprite;
    Rectangle eventRect;
    Vector2 position;
    Vector2 starPosition;
    bool isTriggered;
    bool isCollected;
    EventType eventType;
};
