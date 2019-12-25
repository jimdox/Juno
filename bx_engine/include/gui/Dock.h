#pragma once

namespace bx{
class Dock
{
public:
    Dock();
    ~Dock();
    void init();
    void update(float dt);
    void show_menubar(bool flag);
    void menu_file_dropdown();

private:
    bool f_show_menubar;

// add container for gui elems.

};
}