#include "gui/Dock.h"
#include "render/glRenderCore.h"
#include "entity/Scene.h"
#include "core/Program.h"

extern juno::Program* s_prog_instance;

using namespace juno;

//static Renderer* s_renderer = &Renderer::init(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,0,0));

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

    /* --- style elements --- */
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().ItemSpacing = ImVec2(7,8);


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
    show_side_panel(dt);
    ImGui::ShowDemoWindow();
    show_menubar(true);

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
        // ImGui::MenuItem("n-body-sim.scene");

        ImGui::EndMenu();
    }
    ImGui::Separator();
    if(ImGui::MenuItem("Save", "Ctrl+S")){}
    if(ImGui::MenuItem("Save as")) {}
}

void Dock::show_side_panel(float dt)
{
    bool f_open;
    ImGuiWindowFlags panel_flags = 0;
    panel_flags |= ImGuiWindowFlags_NoTitleBar;
    panel_flags |= ImGuiWindowFlags_NoMove;
    ImGui::Begin(" ", &f_open, panel_flags);    

    ImGui::Text("Frame Time: %.2f", dt); 
    ImGui::Text((const char*)glGetString(GL_VERSION));
    ImGui::Text(" ");
    if(ImGui::CollapsingHeader("Scene"))
    {
        showScenePanel();
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


void Dock::showScenePanel()
{    
    //float v4f[4] = {0, 0, 0, 0};
    if(ImGui::TreeNode("Lights"))
    {
        std::vector<Light> &lights = s_prog_instance->getScene().getLights();
        float input_data[MAX_NUM_LIGHTS][3];
        for(int i = 0; i < lights.size(); i++) 
        {
            ImGui::Text("light %d", i);
            ImGui::Text("  x:%.1f y:%.1f z:%.1f", lights[i].getPosition().x, lights[i].getPosition().y, lights[i].getPosition().z);

            
            // lights[i].setPosition(glm::vec3(input_data[i][0], input_data[i][1], input_data[i][2]));
        }
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Entities"))
    {
        std::vector<Entity>& entities = s_prog_instance->getScene().getEntities();
        for(int i = 0; i < entities.size(); i++)
        {
            ImGui::Text(entities[i].getName().c_str());
            ImGui::Text("  x:%.1f y:%.1f z:%.1f",entities[i].getPosition().x, entities[i].getPosition().y, entities[i].getPosition().z);
        }
        ImGui::TreePop();
    }

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

}

void Dock::showObjectPanel()
{

}

void Dock::showTexturePanel()
{

}

