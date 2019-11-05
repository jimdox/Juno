#include "gui/Dock.h"
#include "render/glRenderCore.h"

using namespace bx;

Dock::Dock()
{

}

void Dock::init()
{

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_FrameBg]                = ImVec4(0.21f, 0.51f, 0.60f, 0.54f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.11f, 0.38f, 0.39f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.49f, 0.78f, 0.86f, 0.25f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.13f, 0.45f, 0.44f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.36f, 0.43f, 0.86f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.22f, 0.22f, 0.22f, 0.97f);


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
    ImGui::ShowDemoWindow();
    ImGui::End();

    ImGui::Render();
    bxRender::renderGui();
}