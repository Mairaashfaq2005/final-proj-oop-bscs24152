#include "pencil_manager.h"
#include "utils.h"
using namespace std;

enum { brush_pen, pencil_pen, fill_pen, polygon_pen };
enum { small = 8, medium = 15, large = 25, xlarge = 35 };

pencil_manager::pencil_manager() {
    pencilbutton = { 1030, 220, 70, 25 };
    int s[4] = { small, medium, large, xlarge };
    for (int i = 0; i < 5; i++) {
        pencilsizes[i] = i + 3;
    }
    for (int i = 0; i < 4; i++) {
        sizebtns[i] = { 1030, 280.0f + (i * 30), 30, 25 };
    }
    currentindex = 0;
}

void pencil_manager::draw() {
    DrawRectangleRec(pencilbutton, (*selectedpen_ptr == pencil_pen) ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(pencilbutton, 1, BLACK);
    DrawText("pencil", pencilbutton.x + 5, pencilbutton.y + 1, 19, BLACK);
    for (int i = 0; i < 4; i++) {
        bool sel = false;
        if (*selectedpen_ptr == pencil_pen && currentindex == i) {
            sel = true;
        }
        DrawRectangleRec(sizebtns[i], sel ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(sizebtns[i], 1, BLACK);
        DrawText(size_label(i).c_str(), sizebtns[i].x + 3, sizebtns[i].y + 1, 19, BLACK);
    }
}

void pencil_manager::update(Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, pencilbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *selectedpen_ptr = pencil_pen;
    }
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointRec(mouse, sizebtns[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            set_size(i);
            log_action("Pencil size selected: " + to_string(get_size()));
        }
    }
}
int pencil_manager::get_size() { 
    return pencilsizes[currentindex]; 
}
void pencil_manager::set_size(int idx) {
    if (idx >= 0 && idx < 4) {
        currentindex = idx;
    }
}
void pencil_manager::set_selectedpen_ptr(int* ptr) {
    selectedpen_ptr = ptr;
}