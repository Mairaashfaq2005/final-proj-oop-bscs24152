#include "writingtools.h"
#include "brush_manager.h"
#include "pencil_manager.h"

writingtools::writingtools() {
    currentindex = 1;
    selectedpen = 0;
    brushmgr = nullptr;
    pencilmgr = nullptr;
    selectedpen_ptr = nullptr;
}
void writingtools::setmanagers(brush_manager* b, pencil_manager* p) {
    brushmgr = b;
    pencilmgr = p;
}
brush_manager* writingtools::getbrushmgr() {
    return brushmgr;
}
pencil_manager* writingtools::getpencilmgr() {
    return pencilmgr;
}
int writingtools::getcurrentind() {
    return currentindex;
}
int writingtools::getselectedpen() {
    return selectedpen;
}
void writingtools::set_selectedpen_ptr(int* ptr) {
    selectedpen_ptr = ptr;
}
int writingtools::getselectedpen() const {
    return *selectedpen_ptr;
}
std::string writingtools::size_label(int i) {
    if (i == 0) return "s";
    if (i == 1) return "m";
    if (i == 2) return "l";
    if (i == 3) return "xl";
    return "?";
}
writingtools::~writingtools() {}