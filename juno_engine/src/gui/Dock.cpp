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
colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
colors[ImGuiCol_WindowBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_FrameBg]                = ImVec4(0.21f, 0.51f, 0.60f, 0.54f);
colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.33f, 0.42f, 0.54f, 0.40f);
colors[ImGuiCol_FrameBgActive]          = ImVec4(0.39f, 0.49f, 0.60f, 0.67f);
colors[ImGuiCol_TitleBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
colors[ImGuiCol_TitleBgActive]          = ImVec4(0.17f, 0.31f, 0.31f, 1.00f);
colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
colors[ImGuiCol_CheckMark]              = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
colors[ImGuiCol_SliderGrab]             = ImVec4(0.42f, 0.55f, 0.73f, 1.00f);
colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
colors[ImGuiCol_Button]                 = ImVec4(0.57f, 0.67f, 0.80f, 0.40f);
colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.61f, 0.80f, 1.00f);
colors[ImGuiCol_ButtonActive]           = ImVec4(0.43f, 0.73f, 0.67f, 1.00f);
colors[ImGuiCol_Header]                 = ImVec4(0.92f, 0.96f, 1.00f, 0.31f);
colors[ImGuiCol_HeaderHovered]          = ImVec4(0.45f, 0.57f, 0.70f, 0.80f);
colors[ImGuiCol_HeaderActive]           = ImVec4(0.41f, 0.55f, 0.72f, 1.00f);
colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
colors[ImGuiCol_ResizeGrip]             = ImVec4(0.49f, 0.78f, 0.86f, 0.25f);
colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.13f, 0.45f, 0.44f, 0.95f);
colors[ImGuiCol_Tab]                    = ImVec4(0.25f, 0.44f, 0.51f, 0.86f);
colors[ImGuiCol_TabHovered]             = ImVec4(0.42f, 0.56f, 0.72f, 0.80f);
colors[ImGuiCol_TabActive]              = ImVec4(0.34f, 0.53f, 0.50f, 1.00f);
colors[ImGuiCol_TabUnfocused]           = ImVec4(0.22f, 0.22f, 0.22f, 0.97f);
colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
colors[ImGuiCol_PlotLines]              = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);




}

Dock::~Dock()
{

}

void Dock::update(float dt)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("debug mode");
    ImGui::Text((const char*)glGetString(GL_VERSION));
    ImGui::Text("frame time: %.3f", dt); 
    //ImGui::Button("button");
    //ImGui::ShowDemoWindow();
    //show_menubar(true);
    ImGui::End();

    ImGui::Render();
    glRender::renderGui();
}


void Dock::show_menubar(bool flag)
{
    if(flag)
    {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
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
        ImGui::MenuItem("n-body.cpp");
        ImGui::MenuItem("string-sim.cpp");
        
        ImGui::EndMenu();
    }
    if(ImGui::MenuItem("Save", "Ctrl+S")){}
    if(ImGui::MenuItem("Save as")) {}
    ImGui::Separator();
    if(ImGui::BeginMenu("Options"))
    {
        bool f_enabled = true;
        ImGui::MenuItem("Enabled", "", &f_enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        ImGui::EndChild();


        ImGui::EndMenu();
    }    

}
