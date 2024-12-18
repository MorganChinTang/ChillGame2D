#include "Event.h"

void Event::Main() {
    Start();

    // Load event sprite
    const char* spritePath = "E:/LaSalle/vgp230_2dGames/2D_ChillGame/Assets/tempEvent.png";
    eventSprite = LoadTexture(spritePath);

    if (eventSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load event sprite: %s", spritePath);
    }

    // Calculate scaled dimensions
    float scale = 0.3f;
    float scaledWidth = eventSprite.width * scale;
    float scaledHeight = eventSprite.height * scale;

    // Set position to middle-right of screen
    position = {
        (float)GetScreenWidth() * 0.75f - scaledWidth / 2.0f,
        (float)GetScreenHeight() / 2.0f - scaledHeight / 2.0f
    };

    // Initialize event rectangle for collision with scaled size
    eventRect = {
        position.x,
        position.y,
        scaledWidth,
        scaledHeight
    };
}

void Event::Start() {
    isTriggered = false;
}

void Event::Update() {
    // Update event rectangle position if needed
    eventRect.x = position.x;
    eventRect.y = position.y;
}

void Event::Draw() {
    if (eventSprite.id != 0) {
        // Draw the event sprite scaled
        Rectangle sourceRec = { 0, 0, (float)eventSprite.width, (float)eventSprite.height };
        Rectangle destRec = { position.x, position.y, eventRect.width, eventRect.height };
        DrawTexturePro(eventSprite, sourceRec, destRec, { 0, 0 }, 0, WHITE);
    }
}

bool Event::CheckCollision(const Player& player) {
    Rectangle playerRect = {
        player.GetPosition().x,
        player.GetPosition().y,
        (float)player.GetFrameWidth(),
        (float)player.GetFrameHeight()
    };

    return CheckCollisionRecs(eventRect, playerRect);
}

void Event::HandleEvent() {
    if (!isTriggered) {
        isTriggered = true;
        // Handle event trigger here
    }
}
