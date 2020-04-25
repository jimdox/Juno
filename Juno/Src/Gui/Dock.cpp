#include "Gui/Dock.h"
#include "Core/Program.h"
#include "Renderer/Renderer.h"
#include "Events/RenderEvents.h"
#include "Core/AssetManager.h"
#include "Core/Config.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


using namespace Juno;
//static Renderer* s_renderer = &Renderer::init(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,0,0));

Dock::Dock() 
{
    f_showMenubar = true;
    f_showStartup = true;
}

void Dock::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

    if(Renderer::Get().GetCurrentGraphicsAPI() == GraphicsAPI::Vulkan)
    {
        /* vulkan stuff */
    } else {
        ImGui_ImplGlfw_InitForOpenGL(Renderer::Get().GetWindow().GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
        JN_INFO("Initialized ImGui OpenGL Context.");
    }

/* "Purple-y theme" */
ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Text]                   = ImVec4(0.99f, 0.99f, 0.99f, 1.00f);
colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
colors[ImGuiCol_WindowBg]               = ImVec4(0.12f, 0.12f, 0.12f, 0.96f);
colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_PopupBg]                = ImVec4(0.20f, 0.20f, 0.20f, 0.94f);
colors[ImGuiCol_Border]                 = ImVec4(0.45f, 0.45f, 0.45f, 0.50f);
colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_FrameBg]                = ImVec4(0.28f, 0.28f, 0.28f, 0.54f);
colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.39f, 0.39f, 0.39f, 0.51f);
colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.39f, 0.39f, 0.51f);
colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
colors[ImGuiCol_TitleBgActive]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
colors[ImGuiCol_MenuBarBg]              = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
colors[ImGuiCol_CheckMark]              = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
colors[ImGuiCol_SliderGrab]             = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
colors[ImGuiCol_Button]                 = ImVec4(0.74f, 0.74f, 0.74f, 0.40f);
colors[ImGuiCol_ButtonHovered]          = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
colors[ImGuiCol_ButtonActive]           = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
colors[ImGuiCol_Header]                 = ImVec4(0.37f, 0.37f, 0.37f, 0.93f);
colors[ImGuiCol_HeaderHovered]          = ImVec4(0.31f, 0.27f, 0.55f, 0.92f);
colors[ImGuiCol_HeaderActive]           = ImVec4(0.31f, 0.27f, 0.55f, 0.92f);
colors[ImGuiCol_Separator]              = ImVec4(0.65f, 0.65f, 0.65f, 0.50f);
colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.65f, 0.65f, 0.65f, 0.78f);
colors[ImGuiCol_SeparatorActive]        = ImVec4(0.65f, 0.65f, 0.65f, 0.94f);
colors[ImGuiCol_ResizeGrip]             = ImVec4(0.12f, 0.12f, 0.12f, 0.25f);
colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.12f, 0.12f, 0.12f, 0.39f);
colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.12f, 0.12f, 0.12f, 0.43f);
colors[ImGuiCol_Tab]                    = ImVec4(0.17f, 0.17f, 0.17f, 0.86f);
colors[ImGuiCol_TabHovered]             = ImVec4(0.30f, 0.21f, 0.60f, 0.80f);
colors[ImGuiCol_TabActive]              = ImVec4(0.30f, 0.22f, 0.60f, 0.80f);
colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.60f, 0.60f, 0.60f, 0.70f);
colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.67f, 0.67f, 0.67f, 0.20f);
colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


/* --- style elements --- */
ImGui::GetStyle().WindowRounding = 0.0f;
ImGui::GetStyle().WindowBorderSize = 0.0f;
ImGui::GetStyle().ScrollbarSize = 14.0f;
ImGui::GetStyle().ItemSpacing = ImVec2(8.0f, 7.0f);
ImGui::GetStyle().FramePadding = ImVec2(8.0f, 3.0f);
ImGui::GetStyle().FrameRounding = 6.0f; 
ImGui::GetStyle().ChildRounding = 6.0f;
ImGui::GetStyle().PopupRounding = 7.0f;
ImGui::GetStyle().PopupBorderSize = 0.0f;

//startupImg = AssetManager::Get().LoadTexture("./Resources/Textures/JunoLogoSmall.png", GL_TEXTURE_2D, TX_DIFFUSE);
}

Dock::~Dock()
{

}

void Dock::Update(SPtr<Scene>& scene, float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if(f_showStartup)
    {
        // if(ImGui::IsAnyMouseDown() && !ImGui::GetIO().WantCaptureMouse || ImGui::IsKeyDown(GLFW_KEY_ESCAPE) || SceneManager::Get().GetScene().GetEntities().size() != 0)
        // {
        //     f_showStartup = false;
        // } else
        // {
        //     ShowStartupWindow();
        // }
    }

    ShowSidePanel(scene, dt);
    ShowMenubar();
    ShowDebugWindow();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    //RenderCall::RenderGui();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Dock::ShowMenubar()
{
    if(f_showMenubar)
    {
        ImGui::GetStyle().FramePadding = ImVec2(8.0f, 2.0f);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                MenuFileDropdown();
                ImGui::EndMenu();   
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Duplicate", "CTRL+D")) {}
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Options"))
            {

                ImGui::EndMenu();
            }
            ImGui::SameLine(1920-65, 0);
            ImGui::Text("v0.1.2d");

            ImGui::EndMainMenuBar();
            }
        ImGui::GetStyle().FramePadding = ImVec2(8.0f, 3.0f);
    }
}

void Dock::MenuFileDropdown()
{
    if(ImGui::MenuItem("New")) {}
    if(ImGui::MenuItem("Open", "Ctrl+O")){}
    if(ImGui::BeginMenu("Open Recent"))
    {
        // ImGui::MenuItem("n-body-sim.scene");

        ImGui::EndMenu();
    }
    ImGui::Separator();
    if(ImGui::MenuItem("Save", "Ctrl+S")){}
    if(ImGui::MenuItem("Save as")) {}
}

void Dock::ShowSidePanel(SPtr<Scene>& scene, float dt)
{
    bool f_open;
    ImGuiWindowFlags panel_flags = 0;
    panel_flags |= ImGuiWindowFlags_NoTitleBar;
    panel_flags |= ImGuiWindowFlags_NoMove;

    ImGui::Begin("SidePanel", &f_open, panel_flags);    

    ImGui::Text((const char*)glGetString(GL_VERSION));
    ImGui::Text("Frame Time: %.2fs", dt);
    ImGui::Text(" ");
    ImGui::Text(" ");
    if(ImGui::CollapsingHeader("Scene"))
    {
        ShowScenePanel(scene);
    }

    if(ImGui::CollapsingHeader("Render"))
    {
        ShowRenderPanel();
    }

    if(ImGui::CollapsingHeader("Physics"))
    {
        ShowPhysicsPanel();
    }


    if(ImGui::CollapsingHeader("Object Data"))
    {
        ShowObjectPanel();
    }

    if(ImGui::CollapsingHeader("Materials"))
    {
        ShowShaderPanel();
    }

    if(ImGui::CollapsingHeader("Shaders"))
    {

    }

    if(ImGui::CollapsingHeader("Textures"))
    {
        ShowTexturePanel();
    }
    ImGui::End();
}


void Dock::ShowScenePanel(SPtr<Scene>& scene)
{    
    ImGuiWindowFlags winFlags = 0;
    winFlags |= ImGuiWindowFlags_HorizontalScrollbar;// (false ? ImGuiWindowFlags_NoScrollWithMouse : 0);
    ImGui::BeginChild("Scene Info", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.98f, 300), true, winFlags);

    //float v4f[4] = {0, 0, 0, 0}
    ImGui::TextColored(ImVec4(0.85, 0.3, 0.3, 1.0), "Lights");
    std::vector<SPtr<Light>>& lights = Renderer::Get().GetCurrentScene()->GetLights();
    float input_data[MAX_NUM_LIGHTS][3];

    for(int i = 0; i < lights.size(); i++) 
    {
        std::string light_name = "light." + std::to_string(i);
        //ImGui::Text("  x:%.1f y:%.1f z:%.1f", scene.getLights()[i].getPosition().x, scene.getLights()[i].getPosition().y, scene.getLights()[i].getPosition().z);
        input_data[i][0] = lights[i]->GetPosition().x;
        input_data[i][1] = lights[i]->GetPosition().y;
        input_data[i][2] = lights[i]->GetPosition().z;
        ImGui::InputFloat3(light_name.c_str(), input_data[i], 1, 0);         
        lights[i]->SetPosition(glm::vec3(input_data[i][0], input_data[i][1], input_data[i][2]));
    }

    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.85, 0.3, 0.3, 1.0), "Entities");

    std::vector<SPtr<Entity>>& entities = Renderer::Get().GetCurrentScene()->GetEntities();
    for(int i = 0; i < entities.size(); i++)
    {
        ImGui::TextColored(ImVec4(0.2f, 0.72f, 0.75f, 1.0f), entities[i]->GetName().c_str());
        ImGui::Text("  x:%.1f y:%.1f z:%.1f",entities[i]->GetPosition().x, entities[i]->GetPosition().y, entities[i]->GetPosition().z);
    }
    
    ImGui::EndChild();
}

void Dock::ShowRenderPanel()
{   
    ImGui::Text("Renderer Mode");
    if(ImGui::Combo("", &rendererSelector, "OpenGL\0Vulkan\0"))
    {
        // switch(renderer_selector)
        // {
        // case 0: // switch to openGl    // break;
        // case 1: // switch to openGl ES // break;
        // case 2: // switch to Vulkan    // break;
        // }
    }

    if(ImGui::Checkbox("Wireframe Mode", &renderEffectWireFrame))
    {
        Notify(RenderWireFrameEvent(renderEffectWireFrame));
    } 

    // float clearColorVal = Renderer::Get().GetClearColorBrightness();
    // ImGui::SliderFloat(" ", &clearColorVal, 0.0f, 1.0f, "Clear Color = %.3f");
    // Renderer::Get().SetClearColorBrightness(clearColorVal);


}


void Dock::ShowPhysicsPanel()
{

}

void Dock::ShowShaderPanel()
{
    ImGuiWindowFlags winFlags = 0;
    winFlags |= ImGuiWindowFlags_HorizontalScrollbar;// (false ? ImGuiWindowFlags_NoScrollWithMouse : 0);
    ImGui::BeginChild("Loaded Shaders", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 200), true, winFlags);
    
    // std::map<unsigned int, std::unique_ptr<Shader>>& shaderMap = *AssetManager::Get().GetShaderMap();
    // for(auto itr = shaderMap.begin(); itr != shaderMap.end(); ++itr)   
    // {
    //     ImGui::TextColored(ImVec4(0.28f, 0.78f, 0.83f, 1.0f), "Shader:%s", itr->second->GetFilepath().c_str());

    // }
    ImGui::EndChild();
}

void Dock::ShowObjectPanel()
{

}

void Dock::ShowTexturePanel()
{

    ImGuiWindowFlags winFlags = 0;
    winFlags |= ImGuiWindowFlags_HorizontalScrollbar;// (false ? ImGuiWindowFlags_NoScrollWithMouse : 0);
    ImGui::BeginChild("Loaded Textures", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 300), true, winFlags);
    
    // std::map<unsigned int, std::unique_ptr<Texture>>& texMap = *AssetManager::Get().GetTextureMap();
    // for(auto itr = texMap.begin(); itr != texMap.end(); ++itr)   
    // {
    //     //ImGui::TextColored(ImVec4(0.28f, 0.78f, 0.83f, 1.0f), "Texture %d", itr->second->GetID()-1);

    //     //ImGui::Image((void*)(intptr_t)itr->second->GetID(), ImVec2(120, 120));
    // }

    // for (int i = 0; i < 100; i++)
    // {
    //     ImGui::Text("%04d: scrollable region", i);
    //     // if (goto_line && line == i)
    //     //     ImGui::SetScrollHereY();
    // }
    // if (goto_line && line >= 100)
    //     ImGui::SetScrollHereY();
    ImGui::EndChild();
}

void Dock::EnableDebug()
{
    f_debug = true;
}

void Dock::PrintToDebug(const char* str) 
{
    debugBuffer.push_back(str);
}

void Dock::ShowDebugWindow()
{
    if(f_debug)
    {
        bool f_open;
        ImGuiWindowFlags win_flags = 0;
        //win_flags |= ImGuiWindowFlags_NoTitleBar;
        // win_flags |= ImGuiWindowFlags_NoMove;
        // win_flags |= ImGuiWindowFlags_NoResize;
        ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiCond_Always);
        ImGui::Begin("Debug", &f_open, win_flags);
        for(unsigned int i = 0; i < debugBuffer.size(); i++)
        {
            ImGui::Text(debugBuffer[i]);
        }
        debugBuffer.clear();

        ImGui::End();
    }
}

void Dock::ShowStartupWindow()
{
    bool f_open;
    ImGuiWindowFlags win_flags = 0;
    win_flags |= ImGuiWindowFlags_NoTitleBar;
    win_flags |= ImGuiWindowFlags_NoMove;
    win_flags |= ImGuiWindowFlags_NoResize;
    
    ImGui::SetNextWindowPos(ImVec2(1920/2,1080/2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("welcome menu", &f_open, win_flags);
    ImGui::Text("[esc to hide] \t\t\t   v%s", Juno::JN_VERSION);
    ImGui::Image((void*)(intptr_t)id_startupImg, ImVec2(256, 213));
    //ImGui::Text("You can follow the project @ github.com/jimdox/Juno");
    ImGui::Text("Templates:\t\t\t Recent Files:");
    ImGui::Separator();
    ImGui::Columns(2);

    std::vector<std::string> template_programs; 
    std::vector<std::string> recent_programs;
    template_programs.push_back("General Scene"); 
    template_programs.push_back("Particle System");

    for(int i = 0; i < template_programs.size(); i++)
    {
        if(ImGui::Selectable(template_programs[i].c_str(), false))
        {
            // if(i == 2)
            // {
            //     f_show_startup = false;
            //     ImGui::End();
            //     ImGui::EndFrame();
            //     notify(CustomWindowEvent(2));
            // } else if (i == 1)
            // {

            // } else {
            //     f_show_startup = false;
            //     ImGui::End();
            //     ImGui::EndFrame();
            //     notify(CustomWindowEvent(0));
            // }
            f_showStartup = false;
            ImGui::End();
            ImGui::EndFrame();
            //Notify(CustomWindowEvent(i));
        }
    }
    ImGui::NextColumn();
    for(int i = 0; i < recent_programs.size(); i++)
    {
        if(ImGui::Selectable(recent_programs[i].c_str()))
        {

        }
    }

    ImGui::End();        
}