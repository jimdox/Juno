#pragma once
namespace juno {
class Dock
{
public:
    Dock();
    ~Dock();
    void init();
    void update(float dt);
    void show_menubar(bool flag);
    void show_side_panel(bool flag, float dt);
    void menu_file_dropdown();

private:
    bool f_show_menubar;
    int renderer_selector;
    int render_effect_shadow;
    bool render_effect_bloom;

    

// add container for gui elems.

};
}