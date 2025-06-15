#pragma once
#include "raylib.h"
#include <string>
using namespace std;

class canvas {
private:
    RenderTexture2D target;
    static const int maxstates = 100;
    Image states[maxstates];
    Image undostates[maxstates];
    int undocount;
    int stateindex;
    int statecount;
    int width, height;
public:
    canvas(int w, int h);
    void clear();
    void push_state();
    void draw_at(Vector2 pos, float radius, Color c, int pen_type);
    void render();
    void undo();
    void redo();
    void unload();
    void save_image(const string& filename);
    void load_image(const string& filename);
    void reset_last_mouse();
    RenderTexture2D gettarget();
    void draw_polygon(Vector2 center, float radius, int sides, Color c);
    void save_state(const string& file);
    void load_state(const string& file);
    void fill(Vector2 pos, RenderTexture2D target, Color replacement);
};