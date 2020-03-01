#pragma once
#include "events/Event.h"

namespace juno {
class Dock : EventDispatcher
{
public:
    Dock();
    ~Dock();
    void init();
    void update(float dt);
    void show_menubar(bool flag);
    void show_side_panel(bool flag, float dt);
    void menu_file_dropdown();

    void show_side_panel(float dt);
    void showScenePanel();
    void showRenderPanel();
    void showPhysicsPanel();
    void showShaderPanel();
    void showMeshPanel();
    void showTexturePanel();

private:
    bool f_show_menubar;
    bool f_show_sidepanel;
    int render_effect_shadow;
    int renderer_selector;
    // Program* prog;


// add container for gui elems.

};
}