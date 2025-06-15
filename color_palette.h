#pragma once
#include "raylib.h"

class color_palette {
private:
    Color colors[23];
    Rectangle colorbuttons[23];
    int selectedcolour;
    int hoveredcolour;
public:
    color_palette();
    void update(Vector2 mouse);
    void draw();
    Color get_current_color();
};