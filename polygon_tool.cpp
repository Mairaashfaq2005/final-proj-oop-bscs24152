#include "polygon_tool.h"
#include "utils.h"
using namespace std;

polygon_tool::polygon_tool() {
    polybtn = { 970, 455, 90, 30 };
    incbutton = { 970, 510, 20, 20 };
    decbutton = { 950, 510, 20, 20 };
    sides = 3;
    active = false;
}
void polygon_tool::draw_polygon(Vector2 center, float radius, Color c) {
    Vector2 points[21];
    for (int i = 0; i < sides; i++) {
        float angle = 2 * PI * i / sides - PI / 2;
        points[i] = { center.x + radius * cosf(angle), center.y + radius * sinf(angle) };
    }
    for (int i = 0; i < sides; i++) {
        DrawLineV(points[i], points[(i + 1) % sides], c);
    }
}
void polygon_tool::draw() {
    DrawRectangleRec(polybtn, active ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(polybtn, 2, LIGHTGRAY);
    DrawText("polygon", polybtn.x + 5, polybtn.y + 1, 19, BLACK);
    DrawRectangleRec(incbutton, LIGHTGRAY);
    DrawRectangleRec(decbutton, LIGHTGRAY);
    DrawText("+", incbutton.x + 5, incbutton.y + 1, 19, BLACK);
    DrawText("-", decbutton.x + 5, decbutton.y + 1, 19, BLACK);
    DrawText(("n = " + to_string(sides)).c_str(), 950, decbutton.y + 20, 18, BLACK);
}
void polygon_tool::update(Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, polybtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        active = true;
        log_action("Polygon tool selected");
    }
    if (CheckCollisionPointRec(mouse, incbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        if (sides < 20) sides++;
    if (CheckCollisionPointRec(mouse, decbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        if (sides > 3) sides--;
}
bool polygon_tool::getstatus() {
    return active;
}
void polygon_tool::setstatus(bool s) { 
    active = s;
}
int polygon_tool::getsides() { 
    return sides;
}
void polygon_tool::setsides(int s) { 
    sides = s; 
}