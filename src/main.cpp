#include "webvisualize/context.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    Context& context = Context::GetInstance();
    if (!context.Initialize()) {
        std::cout << "Unable to initialize context" << std::endl;
        return -1;
    }

    context.Run();

    return 0;
}

