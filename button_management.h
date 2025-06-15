#pragma once
#include "raylib.h"
#include "canvas.h"

class button_management {
public:
    Rectangle save_btn, load_btn, clear_btn, undo_btn, redo_btn;
    bool hover_save, hover_load, hover_clear, hover_undo, hover_redo;
    bool show_saved_msg, show_loaded_msg;
    int save_counter, load_counter;

    button_management();
    void update(Vector2 mouse, canvas& can);
    void draw();
};
