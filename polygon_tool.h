#pragma once
#include "raylib.h"

class polygon_tool {
private:
    Rectangle polybtn;
    Rectangle incbutton;
    Rectangle decbutton;
    int sides;
    bool active;
public:
    polygon_tool();
    void draw_polygon(Vector2 center, float radius, Color c);
    void draw();
    void update(Vector2 mouse);
    bool getstatus();
    void setstatus(bool s);
    int getsides();
    void setsides(int s);
};
