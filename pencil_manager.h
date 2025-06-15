#pragma once
#include "writingtools.h"
#include "raylib.h"

class pencil_manager : public writingtools {
protected:
    Rectangle pencilbutton;
    Rectangle sizebtns[4];
    int pencilsizes[4];
    int currentindex;
    int* selectedpen_ptr;
public:
    pencil_manager();
    void draw() override;
    void update(Vector2 mouse) override;
    int get_size();
    void set_size(int idx);
    void set_selectedpen_ptr(int* ptr);
};