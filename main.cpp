#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;


void log_action(const string& msg) {
    ofstream log_file("log.txt", ios::app);
    if (log_file.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        log_file << "[" << dt << "] " << msg << endl;
        log_file.close();
    }
}

template <typename T>
void safe_write(const string& filename, T* data, size_t count) {
    try {
        ofstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("cannot open file for writing");
        }
        file.write(reinterpret_cast<const char*>(data), sizeof(T) * count);
        file.close();
    }
    catch (const exception& e) {
        log_action(string("Error writing file "));
    }
}
template <typename T>
void safe_read(const string& filename, T* data, size_t count) {
    try {
        ifstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("cannot open file for reading");
        }
        file.read(reinterpret_cast<char*>(data), sizeof(T) * count);
        file.close();
    }
    catch (const exception& e) {
        log_action(string("Error reading file"));
    }
}



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
        return "?";
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
        brushbutton = { 950, 80, 70, 25};
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
                log_action("Brush size selected: " + to_string(get_size()));
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
        pencilbutton = { 950, 250, 50, 25 };
        int s[4] = { small, medium, large, xlarge };
        for (int i = 0; i < 4; i++) {
            pencilsizes[i] = s[i] - ((i + 1) * 4);;
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
            bool sel = false;
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
                log_action("Pencil size selected: " + to_string(get_size()));
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
            log_action("Fill tool selected");
        }
    }
    void draw(bool fill_selected) {
        DrawRectangleRec(fill_btn, fill_selected ? DARKGRAY : LIGHTGRAY);
        DrawRectangleLinesEx(fill_btn, 1, BLACK);
        DrawText("fill", fill_btn.x + 1, fill_btn.y + 1, 19, BLACK);
    }

    template<typename T>
    void flood_fill(Image& img, int x, int y, T target, T replacement) {
        if (x < 0 || x >= img.width || y < 0 || y >= img.height) { // boundary chk
            return;
        }
        Color* pixels = (Color*)img.data;
        T* p = (T*)img.data;
        if (p[y * img.width + x] == replacement) {
            return;
        }
        if (p[y * img.width + x] != target) {
            return;
        }
        p[y * img.width + x] = replacement;
        flood_fill(img, x + 1, y, target, replacement);
        flood_fill(img, x - 1, y, target, replacement);
        flood_fill(img, x, y + 1, target, replacement);
        flood_fill(img, x, y - 1, target, replacement);
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
            log_action("Polygon tool selected");
        }
        if (CheckCollisionPointRec(mouse, incbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            if (sides < 20) sides++;
        if (CheckCollisionPointRec(mouse, decbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            if (sides > 3) sides--;
    }

    bool getstatus() {
        return active;
    }
    void setstatus(bool s) {
        active = s;
    }
    int getsides() {
        return sides;
    }
    void setsides(int s) {
        sides = s;
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
            log_action("State pushed");
        }
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
            log_action("Undo"); 
        }
    }
    void redo() {
        if (stateindex < statecount - 1) {
            stateindex++;
            BeginTextureMode(target);
            DrawTexture(LoadTextureFromImage(states[stateindex]), 0, 0, WHITE);
            EndTextureMode();
            log_action("Redo"); 
        }
    }

    void unload() {
        for (int i = 0; i < statecount; i++) {
            if (states[i].data) UnloadImage(states[i]);
        }
        UnloadRenderTexture(target);
    }

    void save_image(const string& filename) {
        Image img = LoadImageFromTexture(target.texture);
        ExportImage(img, filename.c_str());
        UnloadImage(img);
        log_action("Image saved: " + filename);
    }
    void load_image(const string& filename) {
        Image img = LoadImage(filename.c_str());
        if (img.data) {
            BeginTextureMode(target);
            DrawTexture(LoadTextureFromImage(img), 0, 0, WHITE); 
            EndTextureMode();
            UnloadImage(img);
            log_action("Image loaded: " + filename);
        }
    }

    void reset_last_mouse() {
        BeginTextureMode(target);
        EndTextureMode();
    }

    RenderTexture2D gettarget() {
        return target;
    }

    void draw_polygon(Vector2 center, float radius, int sides, Color c) {
        BeginTextureMode(target);
        Vector2 points[21];
        for (int i = 0; i < sides; i++) {
            float angle = 2.0f * PI * i / sides - PI / 2.0f;
            points[i] = { center.x + radius * cosf(angle), center.y + radius * sinf(angle) };
        }
        for (int i = 0; i < sides; i++) {
            DrawLineV(points[i], points[(i + 1) % sides], c);
        }
        EndTextureMode();
    }

    void save_state(const string& file) {
        Image img = LoadImageFromTexture(target.texture);
        safe_write<char>(file, static_cast<char*>(img.data), img.width * img.height * 4);
        UnloadImage(img);
        log_action("Canvas state saved to: " + file);
    }
    void load_state(const string& file) {
        Image img = GenImageColor(width, height, RAYWHITE);
        safe_read<char>(file, static_cast<char*>(img.data), width * height * 4);
        BeginTextureMode(target);
        DrawTexture(LoadTextureFromImage(img), 0, 0, WHITE);
        EndTextureMode();
        UnloadImage(img);
        log_action("Canvas state loaded from: " + file);
    }


    void fill(Vector2 pos, RenderTexture2D target, Color replacement) {
        Image img = LoadImageFromTexture(target.texture);
        int x = (int)pos.x, y = (int)pos.y;
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        Color* pixels = (Color*)img.data;
        Color orig = pixels[y * img.width + x];
        if (ColorToInt(orig) == ColorToInt(replacement)) {
            UnloadImage(img);
            return;
        }
        int n = 0;
        do {
            n = 0;
            for (int i = 0; i < img.width * img.height; i++) {
                if (ColorToInt(pixels[i]) == ColorToInt(orig)) {
                    if (i > 0 && ColorToInt(pixels[i - 1]) == ColorToInt(replacement)) {
                        pixels[i] = replacement; n++;
                    }
                    if (i < img.width * img.height - 1 && ColorToInt(pixels[i + 1]) == ColorToInt(replacement)) {
                        pixels[i] = replacement; n++; 
                    }
                    if (i >= img.width && ColorToInt(pixels[i - img.width]) == ColorToInt(replacement)) { 
                        pixels[i] = replacement; n++; 
                    }
                    if (i < img.width * (img.height - 1) && ColorToInt(pixels[i + img.width]) == ColorToInt(replacement)) { 
                        pixels[i] = replacement; n++; 
                    }
                }
            }
        } while (n > 0);
        Texture2D newtex = LoadTextureFromImage(img);
        BeginTextureMode(target);
        DrawTexture(newtex, 0, 0, WHITE);
        EndTextureMode();
        UnloadTexture(newtex);
        UnloadImage(img);
    }

};

class button_management {
public :
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


    void update(Vector2 mouse, canvas& can) {
        hover_save = CheckCollisionPointRec(mouse, save_btn);
        hover_load = CheckCollisionPointRec(mouse, load_btn);
        hover_clear = CheckCollisionPointRec(mouse, clear_btn);
        hover_undo = CheckCollisionPointRec(mouse, undo_btn);
        hover_redo = CheckCollisionPointRec(mouse, redo_btn);

        if ((hover_save && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_S)) {
            can.save_image("pic.png");
            show_saved_msg = true;
            save_counter=0;
        }
        if ((hover_load && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_L)) {
            can.load_image("pic.png");
            show_loaded_msg = true;
            load_counter=0;
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

    void draw() {
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

    int pen_type = brush_pen;
    bool fill_selected = false;
    bool polygon_selected = false;
    int current_tool = 0;

    Vector2 poly_center = { 0, 0 };
    float poly_radius = 50;

    while (!WindowShouldClose()) { //(KEY_ESCAPE pressed or windows close icon clicked)
        Vector2 mouse = GetMousePosition();
        palette.update(mouse);

        for (int i = 0; i < 2; i++) {
            tool_ptrs[i]->update(mouse);
        }
        if (brush_mgr.getselectedpen() == brush_pen) {
            pen_type = brush_pen;
        }
        else {
            pen_type = pencil_pen;
        }
        current_tool = (pen_type == brush_pen) ? 0 : 1;
        fillmgr.update(mouse, fill_selected);
        polymgr.update(mouse);
        polygon_selected = polymgr.getstatus();
        ui.update(mouse, canv);

        if (fill_selected || polygon_selected) {
            canv.reset_last_mouse();
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouse.x < 940 && mouse.y < 690) {
            if (fill_selected) {
                canv.push_state();
                Image img = LoadImageFromTexture(canv.gettarget().texture);
                Color clicked = GetImageColor(img, (int)mouse.x, (int)mouse.y);
                canv.fill(mouse, canv.gettarget(), palette.get_current_color());
                UnloadImage(img);
                fill_selected = false;
            }
            else if (polygon_selected) {
                canv.push_state();
                poly_center = mouse;
                canv.draw_polygon(poly_center, poly_radius, polymgr.getsides(), palette.get_current_color());
                polymgr.setstatus(false);
            }
            else {
                canv.push_state();
                Color draw_color = palette.get_current_color();
                float size;
                if (pen_type == brush_pen) {
                    size = brush_mgr.get_size();
                }
                else if (pen_type == pencil_pen) {
                    size = pencil_mgr.get_size();
                }
                canv.draw_at(mouse, size, draw_color, pen_type);
            }
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) canv.reset_last_mouse();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        canv.render();

        // Preview
        if (mouse.x < 940 && mouse.y < 690) {
            float preview_size = (pen_type == brush_pen) ? brush_mgr.get_size() : pencil_mgr.get_size();
            if (polygon_selected)
                polymgr.draw_polygon(mouse, poly_radius, palette.get_current_color());
            else if (pen_type == pencil_pen)
                DrawCircle(mouse.x, mouse.y, 2, Fade(palette.get_current_color(), 0.5f));
            else
                DrawCircleV(mouse, preview_size, Fade(palette.get_current_color(), 0.5f));
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
