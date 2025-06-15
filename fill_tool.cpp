#include "fill_tool.h"
#include "utils.h"

fill_tool::fill_tool() {
    fill_btn = { 1030, 400, 50, 25 };
}
void fill_tool::update(Vector2 mouse, bool& fill_selected) {
    if (CheckCollisionPointRec(mouse, fill_btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        fill_selected = true;
        log_action("Fill tool selected");
    }
}
void fill_tool::draw(bool fill_selected) {
    DrawRectangleRec(fill_btn, fill_selected ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(fill_btn, 1, BLACK);
    DrawText("fill", fill_btn.x + 15, fill_btn.y + 1, 19, BLACK);
}

