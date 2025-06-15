#include "canvas.h"
#include "utils.h"
#include "raylib.h"
#include <iostream>

enum { brush_pen, pencil_pen, fill_pen, polygon_pen };

canvas::canvas(int w, int h) {
    width = w;
    height = h;
    target = LoadRenderTexture(width, height);
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();
    stateindex = -1;
    statecount = 0;
    undocount = -1;
}

void canvas::clear() {
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();
}

void canvas::push_state() {
    Image img = LoadImageFromTexture(target.texture);
    if (stateindex < statecount - 1) {
        for (int i = stateindex + 1; i < statecount; i++) {
            if (states[i].data) UnloadImage(states[i]);
        }
        statecount = stateindex + 1;
        undocount = -1;
    }
    if (stateindex < maxstates) {
        stateindex++;
        states[stateindex] = ImageCopy(img);
        statecount++;
    }
    else {
        UnloadImage(states[0]);
        for (int i = 1; i < maxstates; i++) {
            states[i - 1] = states[i];
        }
        states[maxstates - 1] = ImageCopy(img);
    }
    UnloadImage(img);
    undocount = -1;
    log_action("State pushed");
}

void canvas::draw_at(Vector2 pos, float radius, Color c, int pen_type) {
    static Vector2 last_mouse = { -1, -1 };
    if (pos.x > 940 || pos.y < 20) return;
    BeginTextureMode(target);
    if (pen_type == brush_pen) {
        DrawCircleV(pos, radius, c);
    }
    else if (pen_type == pencil_pen) {
        if (last_mouse.x >= 0 && last_mouse.y >= 0)
            DrawCircleV(pos, radius, c);
    }
    EndTextureMode();
    last_mouse = pos;
}
void canvas::render() {
    DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height }, { 0, 0 }, WHITE);
}

void canvas::undo() {
    if (stateindex >= 0) {
        undocount++;
        undostates[undocount] = ImageCopy(states[stateindex]);
        stateindex--;
        Image img = ImageCopy(states[stateindex]);
        ImageFlipVertical(&img);
        Texture2D tex = LoadTextureFromImage(img);
        BeginTextureMode(target);
        DrawTexture(tex, 0, 0, WHITE);
        EndTextureMode();
        UnloadTexture(tex);
        UnloadImage(img);
        log_action("Undo");
    }
}

void canvas::redo() {
    if (undocount >= 0 && stateindex < maxstates - 1) {
        stateindex++;
        states[stateindex] = ImageCopy(undostates[undocount]);
        undocount--;
        if (stateindex >= statecount) {
            statecount = stateindex + 1;
        }
        Image img = ImageCopy(states[stateindex]);
        ImageFlipVertical(&img);
        Texture2D tex = LoadTextureFromImage(img);
        BeginTextureMode(target);
        DrawTexture(tex, 0, 0, WHITE);
        EndTextureMode();
        UnloadTexture(tex);
        UnloadImage(img);
        log_action("Redo");
    }
}

void canvas::unload() {
    for (int i = 0; i < statecount; i++) {
        if (states[i].data) UnloadImage(states[i]);
    }
    for (int i = 0; i < +undocount; i++) {
        if (undostates[i].data) UnloadImage(undostates[i]);
    }
    UnloadRenderTexture(target);
}

void canvas::save_image(const std::string& filename) {
    Image img = LoadImageFromTexture(target.texture);
    ExportImage(img, filename.c_str());
    UnloadImage(img);
    log_action("Image saved: " + filename);
}
void canvas::load_image(const std::string& filename) {
    Image img = LoadImage(filename.c_str());
    if (img.data) {
        Image img2 = ImageCopy(states[stateindex]);
        ImageFlipVertical(&img2);
        Texture2D tex = LoadTextureFromImage(img2);
        BeginTextureMode(target);
        DrawTexture(tex, 0, 0, WHITE);
        EndTextureMode();
        UnloadTexture(tex);
        UnloadImage(img2);
        log_action("Image loaded: " + filename);
    }
    UnloadImage(img);
}

void canvas::reset_last_mouse() {
    BeginTextureMode(target);
    EndTextureMode();
}

RenderTexture2D canvas::gettarget() {
    return target;
}

void canvas::draw_polygon(Vector2 center, float radius, int sides, Color c) {
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

void canvas::save_state(const std::string& file) {
    Image img = LoadImageFromTexture(target.texture);
    safe_write<char>(file, static_cast<char*>(img.data), img.width * img.height * 4);
    UnloadImage(img);
    log_action("Canvas state saved to: " + file);
}
void canvas::load_state(const std::string& file) {
    Image img = GenImageColor(width, height, RAYWHITE);
    safe_read<char>(file, static_cast<char*>(img.data), width * height * 4);
    BeginTextureMode(target);
    DrawTexture(LoadTextureFromImage(img), 0, 0, WHITE);
    EndTextureMode();
    UnloadImage(img);
    log_action("Canvas state loaded from: " + file);
}

void canvas::fill(Vector2 pos, RenderTexture2D target, Color replacement) {
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