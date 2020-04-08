#pragma once
#include "events/Event.h"
#include "entity/Scene.h"
namespace juno {
class Dock : public EventDispatcher
{
public:
    Dock();
    ~Dock();
    void init();
    void update(Scene& scene, float dt);
    void show_menubar();
    void show_side_panel(float dt);
    void menu_file_dropdown();

    void show_side_panel(Scene& scene, float dt);
    void showScenePanel(Scene& scene);
    void showRenderPanel();
    void showPhysicsPanel();
    void showShaderPanel();
    void showObjectPanel();
    void showTexturePanel();
    void showStartupWindow();

    void add_window_to_queue();
    void rm_window_from_queue();

private:
    bool f_show_menubar;
    bool f_show_sidepanel;
    bool f_show_startup;
    bool render_effect_wireframe;
    int render_effect_shadow;
    int renderer_selector;
   
    unsigned int startup_img_id;

    std::vector<void (*)() > gui_windows; 
    // Program* prog;


// add container for gui elems.

};
}