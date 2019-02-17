// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------
static Shader myShader;
static unsigned int VBO;
static unsigned int VAO;
static float scale_ratio;
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
unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 600;

// ----------------------------------------------------------------------------
int main(int, char**)
{
  if(hardware_init() != 0)
  {
    return 1;
  }

  // ...
  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,   0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f,  -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f,  -3.5f),
    glm::vec3(-1.7f,  3.0f,  -7.5f),
    glm::vec3( 1.3f, -2.0f,  -2.5f),
    glm::vec3( 1.5f,  2.0f,  -2.5f),
    glm::vec3( 1.5f,  0.2f,  -1.5f),
    glm::vec3(-1.3f,  1.0f,  -1.5f)
  };

  // ...
  scale_ratio = 1.0;
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  // ...
  while(!glfwWindowShouldClose(window))
  {
    static float lastFrame = 0;
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // ...
    glfwPollEvents();
    button_handler();
    gui_draw();

    // Clear the screen
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ...
    myShader.use();

    // Create transformations
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(
      glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f
    );

    // Pass transformation matrices to the shader
    myShader.setMat4("projection", projection);
    myShader.setMat4("view", view);

    static float offset = 0.0;
    offset += deltaTime * 10.0f;

    // render boxes
    glBindVertexArray(VAO);
    for(unsigned int i = 0; i < 10; i++)
    {
      // ...
      glm::mat4 model = glm::mat4(1.0f);

      // ...
      model = glm::translate(model, cubePositions[i]);

      // ...
      float angle = (10.0f * i) + (offset);
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      // ...
      model = glm::scale(model, glm::vec3(scale_ratio));

      // ...
      myShader.setMat4("model", model);

      // ...
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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

    static int counter = 0;
    static ImVec4 clear_color;

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");

    ImGui::SliderFloat("Scale Ratio", &scale_ratio, 0.0f, 1.0f);
    ImGui::ColorEdit3("Example Color", (float*)&clear_color);

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
  SCREEN_WIDTH = width;
  SCREEN_HEIGHT = height;
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
  window = glfwCreateWindow(
    SCREEN_WIDTH, SCREEN_HEIGHT, "Example Window Title", NULL, NULL
  );
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
  myShader.init(
    "../shader/myShader.vs", 
    "../shader/myShader.fs"
  );

  // ...
  if(myShader.isValid == false)
  {
    return -1;
  }

  // ...
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // ...
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW
  );

  // ...
  glVertexAttribPointer(0, 
    3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0
  );
  glEnableVertexAttribArray(0);

  // ...
  glVertexAttribPointer(1, 
    2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))
  );
  glEnableVertexAttribArray(1);

  // ...
  unsigned int tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  
  // Set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Load image, create texture and generate mipmaps
  int width;
  int height;
  int nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(
    "../resource/container.jpg", &width, &height, &nrChannels, 0
  );
  if(data)
  {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // ...
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  // ...
  glEnable(GL_DEPTH_TEST);

  return 0;
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
