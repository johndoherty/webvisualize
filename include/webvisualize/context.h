#ifndef __WEB_VISUALIZE_CONTEXT__
#define __WEB_VISUALIZE_CONTEXT__

#include <GLFW/glfw3.h>

class Context {
public:
    static Context& GetInstance();

    Context() = default;
    ~Context();

    bool Initialize();
    void Run();

    void Resize(int width, int height);

    friend void MainLoop(void *);
private:
    void Loop();
    void ProcessInput();

    int shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    GLFWwindow* window;
};

#endif // __WEB_VISUALIZE_CONTEXT__