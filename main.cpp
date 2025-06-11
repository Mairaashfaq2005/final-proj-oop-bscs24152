#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum pen_type_enum { brush_pen, pencil_pen, fill_pen, polygon_pen };

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

    virtual ~writingtools() {}
};


class brush_manager : public writingtools { //Inherits from writing_tools
protected:
    Rectangle brushbutton;
    Rectangle sizebutton[4];
    int brushsizes[4];
public:
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
};

class polygon_tool {
private:
    Rectangle polybtn;
    Rectangle incbutton;
    Rectangle decbutton;
    int sides;
    bool active;
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
        selectedcolour = 1;
        hoveredcolour = -1;
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
        height = h;mm
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

};

class button_management {
public:
    Rectangle save_btn, load_btn, clear_btn, undo_btn, redo_btn;
    bool hover_save, hover_load, hover_clear, hover_undo, hover_redo;
    bool show_saved_msg, show_loaded_msg;


    button_management() {
        save_btn = { 950, 560, 45, 22 };
        load_btn = { 1000, 560, 45, 22 };
        clear_btn = { 950, 590, 45, 22 };
        undo_btn = { 1000, 590, 45, 22 };
        redo_btn = { 1050, 590, 45, 22 };
        hover_save = hover_load = hover_clear = hover_undo = hover_redo = false;
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

    while (!WindowShouldClose()) { //(KEY_ESCAPE pressed or windows close icon clicked)
    }
    CloseWindow();
}

