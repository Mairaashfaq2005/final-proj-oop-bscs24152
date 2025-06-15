#include "button_management.h"

button_management::button_management() {
    save_btn = { 970, 560, 45, 22 };
    load_btn = { 1030, 560, 45, 22 };
    clear_btn = { 950, 590, 45, 22 };
    undo_btn = { 1000, 590, 45, 22 };
    redo_btn = { 1050, 590, 45, 22 };
    hover_save = hover_load = hover_clear = hover_undo = hover_redo = false;
    show_saved_msg = show_loaded_msg = false;
    save_counter = load_counter = 0;
}

void button_management::update(Vector2 mouse, canvas& can) {
    hover_save = CheckCollisionPointRec(mouse, save_btn);
    hover_load = CheckCollisionPointRec(mouse, load_btn);
    hover_clear = CheckCollisionPointRec(mouse, clear_btn);
    hover_undo = CheckCollisionPointRec(mouse, undo_btn);
    hover_redo = CheckCollisionPointRec(mouse, redo_btn);

    if ((hover_save && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_S)) {
        can.save_image("pic.png");
        show_saved_msg = true;
        save_counter = 0;
    }
    if ((hover_load && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_L)) {
        can.load_image("pic.png");
        show_loaded_msg = true;
        load_counter = 0;
    }
    if ((hover_clear && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_C)) {
        can.clear();
    }
    if ((hover_undo && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_U)) {
        can.undo();
    }
    if ((hover_redo && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_R)) {
        can.redo();
    }
    if (show_saved_msg && save_counter++ > 100) show_saved_msg = false;
    if (show_loaded_msg && load_counter++ > 100) show_loaded_msg = false;
}

void button_management::draw() {
    DrawRectangleLinesEx(save_btn, 2, hover_save ? RED : BLACK);
    DrawRectangleLinesEx(load_btn, 2, hover_load ? RED : BLACK);
    DrawRectangleLinesEx(clear_btn, 2, hover_clear ? RED : BLACK);
    DrawRectangleLinesEx(undo_btn, 2, hover_undo ? RED : BLACK);
    DrawRectangleLinesEx(redo_btn, 2, hover_redo ? RED : BLACK);
    DrawText("save", save_btn.x + 1, save_btn.y + 1, 15, hover_save ? RED : BLACK);
    DrawText("load", load_btn.x + 1, load_btn.y + 1, 15, hover_load ? RED : BLACK);
    DrawText("clear", clear_btn.x + 1, clear_btn.y + 1, 15, hover_clear ? RED : BLACK);
    DrawText("undo", undo_btn.x + 1, undo_btn.y + 1, 15, hover_undo ? RED : BLACK);
    DrawText("redo", redo_btn.x + 1, redo_btn.y + 1, 15, hover_redo ? RED : BLACK);
    if (show_saved_msg) {
        DrawRectangle(0, 0, 1100, 700, Fade(RAYWHITE, 0.5f));
        DrawRectangle(100, 200, 600, 50, BLACK);
        DrawText("image saved : pic.png", 300, 330, 30, RAYWHITE);
    }
    if (show_loaded_msg) {
        DrawRectangle(0, 0, 1100, 700, Fade(RAYWHITE, 0.5f));
        DrawRectangle(100, 200, 600, 50, BLACK);
        DrawText("image loaded : pic.png", 300, 330, 30, RAYWHITE);
    }
}