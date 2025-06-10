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