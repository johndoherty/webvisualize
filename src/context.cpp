#include "webvisualize/context.h"

#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#include <stdio.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";
const char *fragmentShaderSource = "#version 300 es\n"
    "precision highp float;\n"
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vertexColor, 1.0);\n"
    "}\n\0";

void ResizeWindowCallback(GLFWwindow* window, int width, int height) {
    static_cast<Context*>(glfwGetWindowUserPointer(window))->Resize(width, height);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    static_cast<Context*>(glfwGetWindowUserPointer(window))->MouseButton(button, action, mods);
}

void MainLoop(void * context) {
    static_cast<Context*>(context)->Loop();
}

Context& Context::GetInstance() {
    static Context instance;
    return instance;
}

Context::~Context() {
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

bool Context::Initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window_ == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window_, false);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    ImGui::StyleColorsClassic();

    imgui_ = ImGui::GetCurrentContext();

    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, ResizeWindowCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);

    glfwSetScrollCallback(window_, ImGui_ImplGlfw_ScrollCallback);
    glfwSetKeyCallback(window_, ImGui_ImplGlfw_KeyCallback);
    glfwSetCharCallback(window_, ImGui_ImplGlfw_CharCallback);

    glfwSetWindowUserPointer(window_, this);

    const char * version = (const char *)glGetString(GL_VERSION);
    const char * vendor = (const char *)glGetString(GL_VENDOR);

    std::cout << "Version: " << version << ", Vendor: " << vendor << std::endl;

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    // link shaders
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertexShader);
    glAttachShader(shader_program_, fragmentShader);
    glLinkProgram(shader_program_);
    // check for linking errors
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program_, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return true;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left position  
        1.0f, 0.0f, 0.0f, // bottom left color
        0.5f, -0.5f, 0.0f, // bottom right position
        0.0f, 1.0f, 0.0f, // bottom right color
        0.5f,  0.5f, 0.0f,  // top right position
        0.0f,  0.0f, 1.0f,  // top right color
        -0.5f,  0.5f, 0.0f,  // top left position
        0.5f,  0.5f, 0.0f  // top left color
    };

    unsigned int indices[] = {0u, 1u, 2u, 2u, 0u, 3u};

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered vbo_ as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the vao_ afterwards so other vao_ calls won't accidentally modify this vao_, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    return true;
}

void Context::Loop() {
    ProcessInput();

    // TODO: Make const
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::SetCurrentContext(imgui_);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool show_window = true;

    {
      ImGui::Begin("Debug", &show_window);
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Text("Hello, world!");
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float*)&clear_color);

      if (ImGui::Button("Button")) counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
    glfwPollEvents();
    glfwMakeContextCurrent(window_);
}

void Context::Run() {
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(MainLoop, this, 0, 1);
    #endif
}

void Context::ProcessInput() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) emscripten_run_script("alert('hi')");
}

void Context::Resize(int width, int height) {
    std::cout << "Width: " << width << ", Height: " << height << std::endl;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Context::MouseButton(int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window_, button, action, mods);
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) std::cout << "Clicked!" << std::endl;
}
