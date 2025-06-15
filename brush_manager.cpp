#include "brush_manager.h"
#include "utils.h"

enum { brush_pen, pencil_pen, fill_pen, polygon_pen };
enum { small = 8, medium = 15, large = 25, xlarge = 35 };

brush_manager::brush_manager() {
    brushbutton = { 1030, 50, 70, 25 };
    int s[4] = { small, medium, large, xlarge };
    for (int i = 0; i < 4; i++) {
        brushsizes[i] = s[i];
    }
    for (int i = 0; i < 4; i++) {
        sizebutton[i] = { 1030, 110.0f + (i * 30), 30, 25 };
    }
}

void brush_manager::draw() {
    DrawRectangleRec(brushbutton, (*selectedpen_ptr == brush_pen) ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(brushbutton, 1, BLACK);
    DrawText("brush", brushbutton.x + 5, brushbutton.y + 1, 19, BLACK);
    for (int i = 0; i < 4; i++) {
        bool sel = false;
        if (*selectedpen_ptr == brush_pen && currentindex == i) {
            sel = true;
        }
        DrawRectangleRec(sizebutton[i], sel ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(sizebutton[i], 1, BLACK);
        DrawText(size_label(i).c_str(), sizebutton[i].x + 3, sizebutton[i].y + 1, 19, BLACK);
    }
}

void brush_manager::update(Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, brushbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        *selectedpen_ptr = brush_pen;
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointRec(mouse, sizebutton[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            set_size(i);
            log_action("Brush size selected: " + std::to_string(get_size()));
        }
    }
}

int brush_manager::get_size() { 
    return brushsizes[currentindex];
}
void brush_manager::set_size(int idx) {
    if (idx >= 0 && idx < 4) currentindex = idx;
}
void brush_manager::set_selectedpen_ptr(int* ptr) {
    selectedpen_ptr = ptr;
}