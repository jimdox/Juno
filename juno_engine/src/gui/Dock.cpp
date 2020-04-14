#include "gui/Dock.h"
#include "render/glRenderCore.h"
#include "core/Program.h"
#include "core/EngineConfig.h"

using namespace juno;
//static Renderer* s_renderer = &Renderer::init(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,0,0));

Dock::Dock() 
{
    f_show_menubar = true;
    f_show_startup = true;
}

void Dock::init()
{
    /* "purple-y" theme
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.17f, 0.18f, 0.59f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.34f, 0.82f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.55f, 0.55f, 0.55f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.33f, 0.24f, 0.67f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.60f, 0.60f, 0.60f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.49f, 0.48f, 0.50f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.49f, 0.48f, 0.50f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.71f, 0.67f, 0.93f, 0.31f); 
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.33f, 0.29f, 0.63f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.33f, 0.29f, 0.63f, 0.80f);
    colors[ImGuiCol_Separator]              = ImVec4(0.31f, 0.31f, 0.54f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.33f, 0.44f, 0.64f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.43f, 0.39f, 0.77f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.53f, 0.53f, 0.53f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.27f, 0.26f, 0.88f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.21f, 0.82f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.29f, 0.29f, 0.29f, 0.86f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.34f, 0.27f, 0.77f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.44f, 0.47f, 0.48f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    */
    /* "martian theme" */
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.92f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.44f, 0.19f, 0.19f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.51f, 0.51f, 0.51f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.62f, 0.62f, 0.62f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.55f, 0.55f, 0.55f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.55f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.60f, 0.60f, 0.60f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.49f, 0.48f, 0.50f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.49f, 0.48f, 0.50f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.46f, 0.19f, 0.19f, 0.90f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.46f, 0.19f, 0.19f, 0.67f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.46f, 0.19f, 0.19f, 0.67f);
    colors[ImGuiCol_Separator]              = ImVec4(0.83f, 0.83f, 0.83f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.84f, 0.84f, 0.84f, 0.50f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.53f, 0.53f, 0.53f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.31f, 0.31f, 0.31f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.52f, 0.52f, 0.52f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.29f, 0.29f, 0.29f, 0.86f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.30f, 0.13f, 0.13f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.44f, 0.47f, 0.48f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);




    /* --- style elements --- */
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().ScrollbarSize = 0.6f;
    ImGui::GetStyle().ItemSpacing = ImVec2(8.0f, 7.0f);

    startup_img_id = AssetManager::get().loadTextureFile("./juno_engine/res/juno-s.png", GL_TEXTURE_2D, TX_DIFFUSE);
}

Dock::~Dock()
{

}

void Dock::update(Scene& scene, float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if(f_show_startup)
    {
        if(ImGui::IsAnyMouseDown() && !ImGui::IsAnyWindowHovered() || ImGui::IsKeyDown(GLFW_KEY_ESCAPE))
        {
            f_show_startup = false;
        } else
        {
            showStartupWindow();
        }
    }

    show_side_panel(scene, dt);
    show_menubar();
    showDebugWindow();

    //ImGui::ShowDemoWindow();

    ImGui::Render();
    glRender::renderGui();
}


void Dock::show_menubar()
{
    if(f_show_menubar)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                //ImGui::ShowExampleMenuFile();
                menu_file_dropdown();
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
    }
}

void Dock::menu_file_dropdown()
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

void Dock::show_side_panel(Scene& scene, float dt)
{
    bool f_open;
    ImGuiWindowFlags panel_flags = 0;
    panel_flags |= ImGuiWindowFlags_NoTitleBar;
    panel_flags |= ImGuiWindowFlags_NoMove;
    ImGui::Begin("SidePanel", &f_open, panel_flags);    

    //ImGui::Text("Frame Time: %.2fs", dt);
    ImGui::Text((const char*)glGetString(GL_VERSION));
    ImGui::Text(" ");
    ImGui::Text(" ");
    if(ImGui::CollapsingHeader("Scene"))
    {
        showScenePanel(scene);
    }

    if(ImGui::CollapsingHeader("Render"))
    {
        showRenderPanel();
    }

    if(ImGui::CollapsingHeader("Physics"))
    {
        showPhysicsPanel();
    }

    if(ImGui::CollapsingHeader("Shaders"))
    {
        showShaderPanel();
    }

    if(ImGui::CollapsingHeader("Object Data"))
    {
        showObjectPanel();
    }

    if(ImGui::CollapsingHeader("Textures"))
    {
        showTexturePanel();
    }
    ImGui::End();
}


void Dock::showScenePanel(Scene& scene)
{    

    ImGuiWindowFlags winFlags = 0;
    winFlags |= ImGuiWindowFlags_HorizontalScrollbar;// (false ? ImGuiWindowFlags_NoScrollWithMouse : 0);
    ImGui::BeginChild("Scene Info", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 300), true, winFlags);

    //float v4f[4] = {0, 0, 0, 0}
    ImGui::TextColored(ImVec4(0.85, 0.3, 0.3, 1.0), "Lights");
        std::vector<Light>& lights = scene.getLights();
        float input_data[MAX_NUM_LIGHTS][3];

        for(int i = 0; i < lights.size(); i++) 
        {
            std::string light_name = "light " + std::to_string(i);
            //ImGui::Text("  x:%.1f y:%.1f z:%.1f", scene.getLights()[i].getPosition().x, scene.getLights()[i].getPosition().y, scene.getLights()[i].getPosition().z);
            input_data[i][0] = lights[i].getPosition().x;
            input_data[i][1] = lights[i].getPosition().y;
            input_data[i][2] = lights[i].getPosition().z;
            ImGui::InputFloat3(light_name.c_str(), input_data[i], 1, 0);         
            lights[i].setPosition(glm::vec3(input_data[i][0], input_data[i][1], input_data[i][2]));
        }
        //ImGui::TreePop();
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.85, 0.3, 0.3, 1.0), "Entities");

        std::vector<Entity>& entities = scene.getEntities();
        for(int i = 0; i < entities.size(); i++)
        {
            ImGui::TextColored(ImVec4(0.2f, 0.72f, 0.75f, 1.0f), entities[i].getName().c_str());
            ImGui::Text("  x:%.1f y:%.1f z:%.1f",entities[i].getPosition().x, entities[i].getPosition().y, entities[i].getPosition().z);
        }
        //ImGui::TreePop();
    
    ImGui::EndChild();

}

void Dock::showRenderPanel()
{   
    ImGui::Text("Renderer Mode");
    if(ImGui::Combo("", &renderer_selector, "OpenGL\0OpenGL ES\0"))
    {
        // switch(renderer_selector)
        // {
        // case 0: // switch to openGl    // break;
        // case 1: // switch to openGl ES // break;
        // case 2: // switch to Vulkan    // break;
        // }
    }

    if(ImGui::Checkbox("Wireframe Mode", &render_effect_wireframe))
    {
        notify(RenderWireframeEvent(render_effect_wireframe));
    }


}


void Dock::showPhysicsPanel()
{

}

void Dock::showShaderPanel()
{
    ImGuiWindowFlags winFlags = 0;
    winFlags |= ImGuiWindowFlags_HorizontalScrollbar;// (false ? ImGuiWindowFlags_NoScrollWithMouse : 0);
    ImGui::BeginChild("Loaded Shaders", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 200), true, winFlags);
    
    std::map<unsigned int, std::unique_ptr<Shader>>& shaderMap = *AssetManager::get().getShaderMap();
    for(auto itr = shaderMap.begin(); itr != shaderMap.end(); ++itr)   
    {
        ImGui::TextColored(ImVec4(0.28f, 0.78f, 0.83f, 1.0f), "Shader:%s", itr->second->getFilepath().c_str());

    }
    ImGui::EndChild();
}

void Dock::showObjectPanel()
{

}

void Dock::showTexturePanel()
{

    ImGuiWindowFlags winFlags = 0;
    winFlags |= ImGuiWindowFlags_HorizontalScrollbar;// (false ? ImGuiWindowFlags_NoScrollWithMouse : 0);
    ImGui::BeginChild("Loaded Textures", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 300), true, winFlags);
    
    std::map<unsigned int, std::unique_ptr<Texture>>& texMap = *AssetManager::get().getTextureMap();
    for(auto itr = texMap.begin(); itr != texMap.end(); ++itr)   
    {
        ImGui::TextColored(ImVec4(0.28f, 0.78f, 0.83f, 1.0f), "Texture %d", itr->second->getID()-1);

        ImGui::Image((void*)(intptr_t)itr->second->getID(), ImVec2(120, 120));
    }

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

void Dock::enableDebug()
{
    f_debug = true;
}

void Dock::printToDebug(const char* str) 
{
    debug_buffer.push_back(str);
}

void Dock::showDebugWindow()
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
        for(unsigned int i = 0; i < debug_buffer.size(); i++)
        {
            ImGui::Text(debug_buffer[i]);
        }
        debug_buffer.clear();

        ImGui::End();
    }
}

void Dock::showStartupWindow()
{
        bool f_open;
        ImGuiWindowFlags win_flags = 0;
        win_flags |= ImGuiWindowFlags_NoTitleBar;
        win_flags |= ImGuiWindowFlags_NoMove;
        win_flags |= ImGuiWindowFlags_NoResize;
        
        ImGui::SetNextWindowPos(ImVec2(1920/2,1080/2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("welcome menu", &f_open, win_flags);
        ImGui::Text("[esc to hide] \t\t\t\t\t\t v%s", juno::JN_VERSION);
        ImGui::Image((void*)(intptr_t)startup_img_id, ImVec2(330, 245));
        //ImGui::Text("You can follow the project @ github.com/jimdox/Juno");
        ImGui::Text("Templates:\t\t\t Recent Files:");
        ImGui::Separator();
        ImGui::Columns(2);

        std::vector<std::string> template_programs; 
        std::vector<std::string> recent_programs;
        template_programs.push_back("General Scene"); 
        template_programs.push_back("Simple Physics");
        template_programs.push_back("Particle Systems");

        for(int i = 0; i < template_programs.size(); i++)
        {
            if(ImGui::Selectable(template_programs[i].c_str()))
            {

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

