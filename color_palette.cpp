#include "color_palette.h"

color_palette::color_palette() {
    Color temp[23] = {
        RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
        SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
        LIGHTGRAY, GRAY, DARKGRAY, BLACK
    };
    for (int i = 0; i < 23; i++) {
        colors[i] = temp[i];
    }
    for (int i = 0; i < 23; i++) {
        colorbuttons[i] = { 950.0f, 30.0f + i * 18.0f, 30.0f, 15.0f };
    }
    selectedcolour = 1;
    hoveredcolour = -1;
}
void color_palette::update(Vector2 mouse) {
    for (int i = 0; i < 23; i++) {
        if (CheckCollisionPointRec(mouse, colorbuttons[i])) {
            hoveredcolour = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) selectedcolour = i;
            break;
        }
    }
}
void color_palette::draw() {
    for (int i = 0; i < 23; i++) {
        DrawRectangleRec(colorbuttons[i], colors[i]);
    }
    if (hoveredcolour >= 0) {
        DrawRectangleRec(colorbuttons[hoveredcolour], Fade(WHITE, 0.5f));
    }
    DrawRectangleLinesEx({ colorbuttons[selectedcolour].x, colorbuttons[selectedcolour].y, 35, 19 }, 1, BLACK);
}
Color color_palette::get_current_color() {
    return colors[selectedcolour];
}