#ifndef __WEB_VISUALIZE_CONTEXT__
#define __WEB_VISUALIZE_CONTEXT__

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"

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

    int shader_program_;
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
    GLFWwindow* window_;
    ImGuiContext* imgui_;
};

#endif // __WEB_VISUALIZE_CONTEXT__