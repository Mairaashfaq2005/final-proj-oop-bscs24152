#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include <iostream>
#include "brush_manager.h"
#include "pencil_manager.h"
#include "fill_tool.h"
#include "polygon_tool.h"
#include "color_palette.h"
#include "canvas.h"
#include "button_management.h"
#include "utils.h"
using namespace std;

enum pen_type_enum { brush_pen, pencil_pen, fill_pen, polygon_pen };
enum brush_size_enum { small = 8, medium = 15, large = 25, xlarge = 35 };

int selectedpen = brush_pen;

int main() {
    int width = 1100, height = 700;
    InitWindow(width, height, "bscs24152 oop project");
    SetTargetFPS(120);
    brush_manager brush_mgr;
    pencil_manager pencil_mgr;
    brush_mgr.setmanagers(&brush_mgr, &pencil_mgr);
    pencil_mgr.setmanagers(&brush_mgr, &pencil_mgr);
    writingtools* tool_ptrs[2] = { &brush_mgr, &pencil_mgr };
    brush_mgr.set_selectedpen_ptr(&selectedpen);
    pencil_mgr.set_selectedpen_ptr(&selectedpen);

    fill_tool fillmgr;
    polygon_tool polymgr;
    color_palette palette;
    canvas canv(width - 180, height);
    button_management ui;

    int pen_type;
    bool fill_selected = false;
    bool polygon_selected = false;
    int current_tool = 0;

    Vector2 poly_center = { 0, 0 };
    float poly_radius = 50;
    bool drawing_in_progress = false;
    canv.push_state();

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        palette.update(mouse);
        for (int i = 0; i < 2; i++) {
            tool_ptrs[i]->update(mouse);
        }
        pen_type = selectedpen;
        current_tool = (pen_type == brush_pen) ? 0 : 1;
        fillmgr.update(mouse, fill_selected);
        polymgr.update(mouse);
        polygon_selected = polymgr.getstatus();
        ui.update(mouse, canv);
        pencil_mgr.update(mouse);
        brush_mgr.update(mouse);

        palette.draw();
        brush_mgr.draw();
        pencil_mgr.draw();
        fillmgr.draw(fill_selected);
        polymgr.draw();
        ui.draw();

        if (fill_selected || polygon_selected) {
            canv.reset_last_mouse();
        }

        if (!drawing_in_progress && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouse.x < 940 && mouse.y < 690) {
            drawing_in_progress = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (drawing_in_progress) {
                canv.push_state();  // Save the final result of the stroke
            }
            drawing_in_progress = false;
            canv.reset_last_mouse();
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouse.x < 940 && mouse.y < 690) {
            if (fill_selected) {
                polygon_selected = false;
                selectedpen = fill_pen;
                Image img = LoadImageFromTexture(canv.gettarget().texture);
                Color clicked = GetImageColor(img, (int)mouse.x, (int)mouse.y);
                canv.fill(mouse, canv.gettarget(), palette.get_current_color());
                UnloadImage(img);
                fill_selected = false;
            }
            else if (polygon_selected) {
                fill_selected = false;
                selectedpen = polygon_pen;
                poly_center = mouse;
                canv.draw_polygon(poly_center, poly_radius, polymgr.getsides(), palette.get_current_color());
                polymgr.setstatus(false);
            }
            else {
                Color draw_color = palette.get_current_color();
                float size = 1.0f;
                if (pen_type == brush_pen) {
                    size = brush_mgr.get_size();
                }
                else if (pen_type == pencil_pen) {
                    size = pencil_mgr.get_size();
                }
                canv.draw_at(mouse, size, draw_color, pen_type);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        canv.render();

        // Preview
        if (mouse.x < 940 && mouse.y < 690) {
            int preview_size = (pen_type == brush_pen) ? brush_mgr.get_size() : pencil_mgr.get_size();
            if (polygon_selected)
                polymgr.draw_polygon(mouse, poly_radius, palette.get_current_color());
            else if (pen_type == pencil_pen)
                DrawCircle(mouse.x, mouse.y, preview_size, Fade(palette.get_current_color(), 0.5f));
            else if (pen_type == brush_pen) {
                DrawCircleV(mouse, preview_size, Fade(palette.get_current_color(), 0.5f));
            }
        }
        // Draw toolbar
        DrawRectangle(940, 0, 160, height, Fade(LIGHTGRAY, 0.2f));
        palette.draw();
        brush_mgr.draw();
        pencil_mgr.draw();
        fillmgr.draw(fill_selected);
        polymgr.draw();
        ui.draw();

        // Status
        DrawText(("size: " + to_string((pen_type == brush_pen) ? brush_mgr.get_size() : pencil_mgr.get_size())).c_str(), 950, 640, 15, DARKGRAY);
        DrawText(("mode: " + string(
            fill_selected ? "fill" : (polygon_selected ? "polygon" : ((pen_type == pencil_pen) ? "pencil" : "brush"))
        )).c_str(), 950, 660, 15, DARKGRAY);
        DrawText("Shortcuts: S-save | L-load | C-clear | U-undo | R-redo", 10, 670, 15, BLACK);

        EndDrawing();
    }
    canv.unload();
    CloseWindow();
    return 0;
}