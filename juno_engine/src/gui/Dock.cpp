#include "gui/Dock.h"
#include "render/glRenderCore.h"

using namespace juno;

Dock::Dock()
{
    f_show_menubar = true;
}

void Dock::init()
{
ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Text]                   = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
colors[ImGuiCol_Border]                 = ImVec4(0.17f, 0.18f, 0.59f, 0.50f);
colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_FrameBg]                = ImVec4(0.42f, 0.45f, 0.49f, 0.54f);
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
colors[ImGuiCol_Header]                 = ImVec4(0.58f, 0.56f, 1.00f, 0.29f);
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



/* --- style elements --- */
ImGui::GetStyle().WindowRounding = 0.0f;
ImGui::GetStyle().WindowBorderSize = 0.0f;
ImGui::GetStyle().ScrollbarSize = 0.6f;
ImGui::GetStyle().ItemSpacing = ImVec2(8.0f, 7.0f);
}

Dock::~Dock()
{

}

void Dock::update(float dt)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();
    show_side_panel(true, dt);
    show_menubar(true);

    ImGui::Render();
    glRender::renderGui();
}

void Dock::show_side_panel(bool flag, float dt)
{
    bool p_open = false;
    ImGuiWindowFlags imgui_win_flags = 0;
    imgui_win_flags |= ImGuiWindowFlags_NoTitleBar;
    imgui_win_flags |= ImGuiWindowFlags_NoResize;
    imgui_win_flags |= ImGuiWindowFlags_NoMove;
    ImGui::Begin(" ", &p_open, imgui_win_flags);


    ImGui::Text((const char*)glGetString(GL_VERSION));
    ImGui::Text("Frame Time: %.3f", dt); 

    if(flag)
    {
        if(flag)
        {
            if(ImGui::CollapsingHeader("Scene"))
            {
                
            }
            if(ImGui::CollapsingHeader("Physics"))
            {

            }
            if(ImGui::CollapsingHeader("Renderer"))
            {
                ImGui::Text("Select Rendering SDK");
                if(ImGui::Combo(" ", &renderer_selector, "OpenGL\0Vulkan(Hybrid)\0Raytracer\0"))
                {
                //     switch(renderer_selector)
                //     {
                    // case 0: Renderer::SwitchSDK(SDK_OGL); break;
                    // case 1: Renderer::SwitchSDK(SDK_VULKAN); break;
                    // case 2: Renderer::SwitchSDK(SDK_VULKAN_RAYTRACER); break;
                    
                    // }
                }
                ImGui::Text("\nShadows");
                if(ImGui::SliderInt(" ", &render_effect_shadow, 0, 4))
                {

                }
                if(ImGui::Checkbox("HDR Effects", &render_effect_bloom))
                {

                }

            }
            if(ImGui::CollapsingHeader("Shaders"))
            {

            }
            if(ImGui::CollapsingHeader("Meshes"))
            {

            }
            if(ImGui::CollapsingHeader("Textures"))
            {
                
            }

        }
    }
    ImGui::End();

}


void Dock::show_menubar(bool flag)
{
    if(flag)
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
        // ImGui::MenuItem("n-body.cpp");
        // ImGui::MenuItem("string-sim.cpp");

        ImGui::EndMenu();
    }
    ImGui::Separator();
    if(ImGui::MenuItem("Save", "Ctrl+S")){}
    if(ImGui::MenuItem("Save as")) {}
    

}

