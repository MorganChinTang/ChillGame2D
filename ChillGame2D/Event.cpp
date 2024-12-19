#include "Event.h"

void Event::Start() {
    isCollected = false;
    isTriggered = false;
}

void Event::Update() {
    if (isTriggered && !isCollected) {
        HandleEvent();
    }
}

void Event::HandleEvent() {
    isCollected = true;
}

void Event::Main(int type, Vector2 position) {
    eventType = type;
    eventPosition = position;
    isCollected = false;
    isTriggered = false;

    const char* spritePath = nullptr;
    if (type == 1) {
        spritePath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/susGrass_e1.png";
    }
    else {
        spritePath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/susBerry_e2.png";
    }

    eventSprite = LoadTexture(spritePath);
    const char* starGrayPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/starGray.png";
    const char* starPath = "E:/LaSalle/vgp230_2dGames/ChillGame2D/Assets/star.png";
    starGraySprite = LoadTexture(starGrayPath);
    starSprite = LoadTexture(starPath);
}

bool Event::CheckCollision(const Player& player) {
    if (isCollected || isTriggered) return false;

    Rectangle eventRect = {
        eventPosition.x,
        eventPosition.y,
        eventSprite.width * 7.0f,
        eventSprite.height * 7.0f
    };

    Rectangle playerRect = {
        player.GetPosition().x,
        player.GetPosition().y,
        player.GetFrameWidth(),
        player.GetFrameHeight()
    };

    if (CheckCollisionRecs(eventRect, playerRect)) {
        isTriggered = true;
        return true;
    }
    return false;
}

void Event::Draw() {
    if (!isCollected) {
        Rectangle sourceRec = { 0, 0, (float)eventSprite.width, (float)eventSprite.height };
        Rectangle destRec = { eventPosition.x, eventPosition.y, eventSprite.width * 7.0f, eventSprite.height * 7.0f };
        DrawTexturePro(eventSprite, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
    }

    // Draw stars
    float starScale = 0.3f;
    Rectangle starDestRec1 = { 20, (float)GetScreenHeight() - starGraySprite.height * starScale - 20, starGraySprite.width * starScale, starGraySprite.height * starScale };
    Rectangle starDestRec2 = { 60 + starGraySprite.width * starScale, (float)GetScreenHeight() - starGraySprite.height * starScale - 20, starGraySprite.width * starScale, starGraySprite.height * starScale };

    // Draw appropriate star based on event type and collection status
    if (eventType == 1) {
        DrawTexturePro(isCollected ? starSprite : starGraySprite,
            { 0, 0, (float)starSprite.width, (float)starSprite.height },
            starDestRec1, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        DrawTexturePro(isCollected ? starSprite : starGraySprite,
            { 0, 0, (float)starSprite.width, (float)starSprite.height },
            starDestRec2, { 0, 0 }, 0.0f, WHITE);
    }
}
