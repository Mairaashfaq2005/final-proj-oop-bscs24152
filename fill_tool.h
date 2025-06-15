#pragma once
#include "raylib.h"
#include <string>

class fill_tool {
private:
    Rectangle fill_btn;
public:
    fill_tool();
    void update(Vector2 mouse, bool& fill_selected);
    void draw(bool fill_selected);

    template<typename T>
    void flood_fill(Image& img, int x, int y, T target, T replacement);
};