#include "writingtools.h"
#include "raylib.h"

class brush_manager : public writingtools {
protected:
    Rectangle brushbutton;
    Rectangle sizebutton[4];
    int brushsizes[4];
    int* selectedpen_ptr;
public:
    brush_manager();
    void draw() override;
    void update(Vector2 mouse) override;
    int get_size();
    void set_size(int idx);
    void set_selectedpen_ptr(int* ptr);
};