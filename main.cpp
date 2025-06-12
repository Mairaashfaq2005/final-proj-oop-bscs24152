#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum pen_type_enum { brush_pen, pencil_pen, fill_pen, polygon_pen };
enum brush_size_enum { small = 5, medium = 15, large = 30, xlarge = 45 };

class brush_manager;
class pencil_manager;


class writingtools {
protected: 
    brush_manager* brushmgr;
    pencil_manager* pencilmgr;
    int currentindex;
    int selectedpen;
public: 
    writingtools() {
        currentindex = 1;
        selectedpen = brush_pen;
        brushmgr = nullptr;
        pencilmgr = nullptr;
    }
    //setter
    void setmanagers(brush_manager* b, pencil_manager* p) {
        brushmgr = b;
        pencilmgr = p;
    }
    brush_manager* getbrushmgr() {
        return brushmgr;
    }
    pencil_manager* getpencilmgr() {
        return pencilmgr;
    }
    int getcurrentind() {
        return currentindex;
    }
    int getselectedpen() {
        return selectedpen;
    }

    virtual void update(Vector2 mouse) = 0;
    virtual void draw() = 0;
    virtual string size_label(int i) {
        if (i == 0) return "s";
        if (i == 1) return "m";
        if (i == 2) return "l";
        if (i == 3) return "xl";
        return;
    }

    virtual ~writingtools() {}
};


class brush_manager : public writingtools { //Inherits from writing_tools
protected:
    Rectangle brushbutton;
    Rectangle sizebutton[4];
    int brushsizes[4];
public:
    
    brush_manager() {
        Rectangle brush_btn = { 950, 80, 70, 25};
        int s[4] = { small, medium, large, xlarge };
        for (int i = 0; i < 4; i++) {
            brushsizes[i] = s[i];
        }
        for (int i = 0; i < 4; i++) {
            sizebutton[i] = { 950, 120.0f + (i * 10), 50, 25 };
        }
    }

    void draw() override {
        DrawRectangleRec(brushbutton, (selectedpen == brush_pen) ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(brushbutton, 1, BLACK);
        DrawText("brush", brushbutton.x + 1, brushbutton.y + 1, 19, BLACK);
        for (int i = 0; i < 4; i++) {
            bool sel = false;
            if (selectedpen == brush_pen && currentindex == i) {
                sel = true;
            }
            DrawRectangleRec(sizebutton[i], sel ? DARKGRAY : LIGHTGRAY);
            DrawRectangleLinesEx(sizebutton[i], 1, BLACK);
            DrawText(size_label(i).c_str(), sizebutton[i].x + 1, sizebutton[i].y + 1, 19, BLACK);
        }
    }

    void update(Vector2 mouse) override {
        if (CheckCollisionPointRec(mouse, brushbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selectedpen = brush_pen;
        for (int i = 0; i < 4; i++) {
            if (CheckCollisionPointRec(mouse, sizebutton[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                set_size(i);
            }
        }
    }

    int get_size() {
        return brushsizes[currentindex];
    }
    void set_size(int idx) {
        if (idx >= 0 && idx < 4) currentindex = idx;
    }

};

class pencil_manager : public writingtools {
protected:
    Rectangle pencilbutton;
    Rectangle sizebtns[4];
    int pencilsizes[4];

public:

    pencil_manager() {
        Rectangle pencil_btn = { 950, 250, 50, 25 };
        int s[4] = { small, medium, large, xlarge };
        for (int i = 0; i < 4; i++) {
            pencilsizes[i] = s[i];
        }
        for (int i = 0; i < 4; i++) {
            sizebtns[i] = { 950, 300.0f + (i * 10), 50, 25 };
        }
    }

    void draw() override {
        DrawRectangleRec(pencilbutton, (selectedpen == pencil_pen) ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(pencilbutton, 1, BLACK);
        DrawText("pencil", pencilbutton.x + 1, pencilbutton.y + 1, 19, BLACK);
        for (int i = 0; i < 4; i++) {
            bool sel;
            if (selectedpen == pencil_pen && currentindex == i) {
                sel = true;
            }
            DrawRectangleRec(sizebtns[i], sel ? DARKGRAY : LIGHTGRAY);
            DrawRectangleLinesEx(sizebtns[i], 1, BLACK);
            DrawText(size_label(i).c_str(), sizebtns[i].x + 1, sizebtns[i].y + 1, 19, BLACK);
        }
    }

    void update(Vector2 mouse) override {
        if (CheckCollisionPointRec(mouse, pencilbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selectedpen = pencil_pen;
        for (int i = 0; i < 4; i++) {
            if (CheckCollisionPointRec(mouse, sizebtns[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                set_size(i);
            }
        }
    }

    //string size_label(int i) {
    //    if (i == 0) return "s";
    //    if (i == 1) return "m";
    //    if (i == 2) return "l";
    //    if (i == 3) return "xl";
    //    return;
    //}


    int get_size() {
        return pencilsizes[currentindex];
    }
    void set_size(int idx) {
        if (idx >= 0 && idx < 4) currentindex = idx;
    }
};

class fill_tool {
private:
    Rectangle fill_btn;
public : 
    fill_tool() {
        fill_btn = { 950, 425, 50, 25 };
    }
    void update(Vector2 mouse, bool& fill_selected) {
        if (CheckCollisionPointRec(mouse, fill_btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            fill_selected = true;
        }
    }
    void draw(bool fill_selected) {
        DrawRectangleRec(fill_btn, fill_selected ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(fill_btn, 1, BLACK);
        DrawText("fill", fill_btn.x + 1, fill_btn.y + 1, 19, BLACK);
    }

};

class polygon_tool {
private:
    Rectangle polybtn;
    Rectangle incbutton;
    Rectangle decbutton;
    int sides;
    bool active;
public:
    polygon_tool() {
        polybtn = { 950, 470, 70, 20 };
        incbutton = { 970, 510, 20, 20 };
        decbutton = { 950, 510, 20, 20 };
        sides = 3;
        active = false;
    }
    void draw_polygon(Vector2 center, float radius, Color c) {
        Vector2 points[21];
        for (int i = 0; i < sides; i++) {
            float angle = 2 * PI * i / sides - PI / 2;
            points[i] = { center.x + radius * cosf(angle), center.y + radius * sinf(angle) };
        }
        for (int i = 0; i < sides; i++) {
            DrawLineV(points[i], points[(i + 1) % sides], c);
        }
    }

    void draw() {
        DrawRectangleRec(polybtn, active ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(polybtn, 2, BLACK);
        DrawText("polygon", polybtn.x + 1, polybtn.y + 1, 19, BLACK);
        DrawRectangleRec(incbutton, BLACK);
        DrawRectangleRec(decbutton, BLACK);
        DrawText("+", incbutton.x + 1, incbutton.y, 19, BLACK);
        DrawText("-", decbutton.x + 1, decbutton.y, 19, BLACK);
        DrawText(("n = " + to_string(sides)).c_str(), 975, decbutton.y, 19, BLACK);

    }

    void update(Vector2 mouse) {
        if (CheckCollisionPointRec(mouse, polybtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            active = true;
        }
        if (CheckCollisionPointRec(mouse, incbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            if (sides < 20) sides++;
        if (CheckCollisionPointRec(mouse, decbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            if (sides > 3) sides--;
    }




};

class color_palette {
private:
    Color colors[23];
    Rectangle colorbuttons[23];
    int selectedcolour;
    int hoveredcolour;
public:
    color_palette() {
        Color temp[23] = {
            RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
            SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
            LIGHTGRAY, GRAY, DARKGRAY, BLACK
        };
        for (int i = 0; i < 23; i++) {
            colors[i] = temp[i];
        }

        for (int i = 0; i < 23; i++) {
            colorbuttons[i] = { 950.0f, 30.0f + i * 18.0f, 30.0f, 15.0f };
        }

        selectedcolour = 1;
        hoveredcolour = -1;
    }

    void update(Vector2 mouse) {
        for (int i = 0; i < 23; i++) {
            if (CheckCollisionPointRec(mouse, colorbuttons[i])) {
                hoveredcolour = i; 
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) selectedcolour = i;
                break;
            }
        }
    }
    void draw() {
        for (int i = 0; i < 23; i++) {
            DrawRectangleRec(colorbuttons[i], colors[i]);
        }
        if (hoveredcolour >= 0) {
            DrawRectangleRec(colorbuttons[hoveredcolour], Fade(WHITE, 0.5f));
        }
        DrawRectangleLinesEx({ colorbuttons[selectedcolour].x, colorbuttons[selectedcolour].y, 35, 19 }, 1, BLACK);
    }


    Color get_current_color() {
        return colors[selectedcolour];
    }
};

class canvas {
private:
    RenderTexture2D target;
    // Undo/redo stacks
    static const int maxstates = 100;
    Image states[maxstates];
    int stateindex;
    int statecount;
    int width, height;
public:
    canvas(int w, int h) {
        width = w;
        height = h;
        target = LoadRenderTexture(width, height); //raylib funtion
        BeginTextureMode(target); //raylib function to begin drawing
        ClearBackground(RAYWHITE);
        EndTextureMode();
        stateindex = 0;
        statecount = 0;
    }

    void clear() {
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        EndTextureMode();
    }

    void push_state() {
        Image img = LoadImageFromTexture(target.texture);
        if (statecount < maxstates) {
            states[statecount++] = ImageCopy(img);
            stateindex = statecount - 1;
        }
        else {
            for (int i = 1; i < maxstates; i++) states[i - 1] = states[i];
            states[maxstates - 1] = ImageCopy(img);
            stateindex = maxstates - 1;
        }
        UnloadImage(img);
    }

    void draw_at(Vector2 pos, float radius, Color c, int pen_type) {
        static Vector2 last_mouse = { -1, -1 };
        if (pos.x > 940 || pos.y < 20) return; // block drawing over toolbar
        BeginTextureMode(target);
        if (pen_type == brush_pen) {
            DrawCircleV(pos, radius, c);
        }
        else if (pen_type == pencil_pen) {
            if (last_mouse.x >= 0 && last_mouse.y >= 0)
                DrawLineEx(last_mouse, pos, 1, c);
        }
        EndTextureMode();
        last_mouse = pos;
    }
    void render() {
        DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height }, { 0, 0 }, WHITE);
    }
    void undo() {
        if (stateindex > 0) {
            stateindex--;
            BeginTextureMode(target);
            DrawTexture(LoadTextureFromImage(states[stateindex]), 0, 0, WHITE);
            EndTextureMode();
        }
    }
    void redo() {
        if (stateindex < statecount - 1) {
            stateindex++;
            BeginTextureMode(target);
            DrawTexture(LoadTextureFromImage(states[stateindex]), 0, 0, WHITE);
            EndTextureMode();
        }
    }
    void unload() { 
        UnloadRenderTexture(target); 
    }

};

class button_management {
public:
    Rectangle save_btn, load_btn, clear_btn, undo_btn, redo_btn;
    bool hover_save, hover_load, hover_clear, hover_undo, hover_redo;
    bool show_saved_msg, show_loaded_msg;
    int save_counter, load_counter;

    button_management() {
        save_btn = { 950, 560, 45, 22 };
        load_btn = { 1000, 560, 45, 22 };
        clear_btn = { 950, 590, 45, 22 };
        undo_btn = { 1000, 590, 45, 22 };
        redo_btn = { 1050, 590, 45, 22 };
        hover_save = hover_load = hover_clear = hover_undo = hover_redo = false;
        show_saved_msg = show_loaded_msg = false;
        save_counter = load_counter = 0;
    }


};






int main() {
    int width = 1100, height = 700;
    InitWindow(width, height, "bscs24152 oop project");
    SetTargetFPS(120);
    brush_manager brush_mgr;
    pencil_manager pencil_mgr;
    brush_mgr.setmanagers(&brush_mgr, &pencil_mgr);
    pencil_mgr.setmanagers(&brush_mgr, &pencil_mgr);
    writingtools* tool_ptrs[2] = { &brush_mgr, &pencil_mgr };

    fill_tool fillmgr;
    polygon_tool polymgr;
    color_palette palette;
    canvas canv(width - 180, height);
    button_management ui;


    while (!WindowShouldClose()) { //(KEY_ESCAPE pressed or windows close icon clicked)
        Vector2 mouse = GetMousePosition();
        palette.update(mouse);

    
    
    }

    canv.unload();
    CloseWindow();
    return 0;
}
