#pragma once
#include "raylib.h"
#include <string>

class brush_manager;
class pencil_manager;

class writingtools {
protected:
    brush_manager* brushmgr;
    pencil_manager* pencilmgr;
    int currentindex;
    int selectedpen;
    int* selectedpen_ptr;
public:
    writingtools();
    void setmanagers(brush_manager* b, pencil_manager* p);
    brush_manager* getbrushmgr();
    pencil_manager* getpencilmgr();
    int getcurrentind();
    int getselectedpen();
    void set_selectedpen_ptr(int* ptr);
    int getselectedpen() const;
    virtual void update(Vector2 mouse) = 0;
    virtual void draw() = 0;
    virtual std::string size_label(int i);
    virtual ~writingtools();
};
