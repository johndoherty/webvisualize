#ifndef __WEB_VISUALIZE_CONTEXT__
#define __WEB_VISUALIZE_CONTEXT__

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

class Context {
public:
    static Context& GetInstance();

    Context() = default;
    ~Context();

    bool Initialize();
    void Run();

    void Resize(int width, int height);

    friend void MainLoop(void *);
    friend void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
private:
    void Loop();
    void ProcessInput();
    void MouseButton(int button, int action, int mods);

    int shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    GLFWwindow* window;
    ImGuiContext* imgui;
};

#endif // __WEB_VISUALIZE_CONTEXT__