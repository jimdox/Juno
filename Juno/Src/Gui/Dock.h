#pragma once
#include "Events/Events.h"
#include "Core/Scene.h"
namespace Juno 
{
class Dock : public EventDispatcher
{
public:
    Dock();
    ~Dock();
    void Init();
    void Update(SPtr<Scene>& scene, float dt);
    void ShowMenubar();
    void MenuFileDropdown();

    void ShowSidePanel(SPtr<Scene>& scene, float dt);
    void ShowScenePanel(SPtr<Scene>& scene);
    void ShowRenderPanel();
    void ShowPhysicsPanel();
    void ShowShaderPanel();
    void ShowObjectPanel();
    void ShowTexturePanel();
    void ShowStartupWindow();

    void ShowDebugWindow();
    void EnableDebug();
    void PrintToDebug(const char* str);

    // void add_window_to_queue();
    // void rm_window_from_queue();

private:
    bool f_showMenubar;
    bool f_showSidepanel;
    bool f_showStartup;
    bool f_debug;
    bool renderEffectWireFrame;
    bool showTemplateParticles;
    int renderEffectShadow;
    int rendererSelector;
   
    unsigned int id_startupImg;
    
    std::vector<void (*)() > guiWindows; 
    std::vector<const char*> debugBuffer;
};
}