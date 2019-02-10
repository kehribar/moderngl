// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------
static unsigned int VBO;
static unsigned int VAO;
static GLFWwindow* window;
static bool m_drawGui = false;

// ----------------------------------------------------------------------------
static void gui_draw();
static void gui_render();
static int shader_init();
static int hardware_init();
static void button_handler();
static void hardware_cleanup();
static void glfw_error_callback(int error, const char* description);
static void glfw_size_callback(GLFWwindow* window, int width, int height);

// ----------------------------------------------------------------------------
const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec3 aColor;\n"
  "out vec3 ourColor;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos, 1.0);\n"
  "   ourColor = aColor;\n"
  "}\0";

// ----------------------------------------------------------------------------
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(ourColor, 1.0f);\n"
  "}\n\0";

// ----------------------------------------------------------------------------
int main(int, char**)
{
  if(hardware_init() != 0)
  {
    return 1;
  }

  // ...
  while(!glfwWindowShouldClose(window))
  {
    // ...
    glfwPollEvents();
    button_handler();
    gui_draw();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Render the GUI last!
    gui_render();

    // ...
    glfwSwapBuffers(window);
  }

  // ...
  hardware_cleanup();
  return 0;
}

// ----------------------------------------------------------------------------
static void button_handler()
{
  if(ImGui::GetIO().WantCaptureKeyboard == false)
  {
    static int quake_key_state_d = GLFW_RELEASE;
    int quake_key_state = glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT);
    if((quake_key_state == GLFW_PRESS) && (quake_key_state_d == GLFW_RELEASE))
    {
      m_drawGui = !m_drawGui;
    }
    quake_key_state_d = quake_key_state;
  }
}

// ----------------------------------------------------------------------------
static void gui_draw()
{
  // if(m_drawGui)
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;
    static ImVec4 clear_color;

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);

    if(ImGui::Button("Button"))
    {
      counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
      1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate
    );
    ImGui::End();

    ImGui::Render();
  }
}

// ----------------------------------------------------------------------------
static void gui_render()
{
  if(m_drawGui)
  {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());    
  }
}

// ----------------------------------------------------------------------------
static void glfw_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// ----------------------------------------------------------------------------
static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// ----------------------------------------------------------------------------
static int hardware_init()
{
  // ...
  glfwSetErrorCallback(glfw_error_callback);
  if(!glfwInit())
  {
    return 1;
  }

  // ...
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  window = glfwCreateWindow(1280, 720, "Example Window Title", NULL, NULL);
  if(window == NULL)
  {
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  glfwSetFramebufferSizeCallback(window, glfw_size_callback);

  // ...
  if(gl3wInit() != 0)
  {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // ...
  if(shader_init() != 0)
  {
    return 1;
  }

  return 0;
}

// ----------------------------------------------------------------------------
static int shader_init()
{
  // vertex shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // check for shader compile errors
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
  }

  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
  };

  // ...
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // ...
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glUseProgram(shaderProgram);
}

// ----------------------------------------------------------------------------
static void hardware_cleanup()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}
