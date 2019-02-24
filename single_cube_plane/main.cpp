// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------
static float deltaTime;
static float scale_ratio;
static GLFWwindow* window;
static float rotation_angle;
static bool m_drawGui = true;
static glm::vec3 cubePosition;

// ----------------------------------------------------------------------------
static bool x_rotate;
static bool y_rotate;
static bool z_rotate;

// ----------------------------------------------------------------------------
static Plane_NoTexture myPlane;
static Cube_SixFacesSameTexture myCube;
static Camera camera(glm::vec3(-8.0f, 5.0f, 8.0f));

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
  cubePosition = glm::vec3(3.0f, 0.5f, 3.0f);

  // ...
  scale_ratio = 1.0;
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  // ...
  while(!glfwWindowShouldClose(window))
  {
    // ...
    static float lastFrame = 0;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // ...
    glfwPollEvents();
    button_handler();
    gui_draw();

    // Clear the screen
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create transformations
    view = camera.GetViewMatrix();
    projection = glm::perspective(
      glm::radians(camera.Zoom), 
      (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 
      0.1f, 100.0f
    );

    // ...
    myCube.shader.use();

    // Pass transformation matrices to the shader
    myCube.shader.setMat4("projection", projection);
    myCube.shader.setMat4("view", view);

    // ...
    myCube.bindVertexArray();

    {
      // ...
      glm::mat4 model = glm::translate(
        glm::mat4(1.0f), glm::vec3(-3.0f, 0.5f, -2.0f)
      );

      // ...
      model = glm::scale(model, glm::vec3(1.0));

      // ...
      myCube.shader.setMat4("model", model);

      // ...
      myCube.render();
    }

    {
      // ...
      glm::mat4 model = glm::translate(
        glm::mat4(1.0f), cubePosition
      );

      // ...
      glm::vec3 rotation_vector = glm::vec3(0.0f);

      if(x_rotate)
      {
        rotation_vector.x = 1.0f;
      }

      if(y_rotate)
      {
        rotation_vector.y = 1.0f;
      }

      if(z_rotate)
      {
        rotation_vector.z = 1.0f;
      }

      if(x_rotate || y_rotate || z_rotate)
      {
        model = glm::rotate(
          model, glm::radians(rotation_angle), rotation_vector
        );
      }

      // ...
      model = glm::scale(model, glm::vec3(scale_ratio));

      // ...
      myCube.shader.setMat4("model", model);

      // ...
      myCube.render();
    }

    // ...
    myPlane.shader.use();

    // Pass transformation matrices to the shader
    myPlane.shader.setMat4("projection", projection);
    myPlane.shader.setMat4("view", view);

    // ...
    myPlane.bindVertexArray();

    // ...
    glm::mat4 model = glm::translate(
      glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) 
    );

    // ...
    model = glm::scale(model, glm::vec3(10.0));
    myPlane.shader.setMat4("model", model);

    // ...
    myPlane.render();

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
    // ...
    static int quake_key_state_d = GLFW_RELEASE;
    int quake_key_state = glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT);
    if((quake_key_state == GLFW_PRESS) && (quake_key_state_d == GLFW_RELEASE))
    {
      m_drawGui = !m_drawGui;
    }
    quake_key_state_d = quake_key_state;

    // ...
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
  }
}

// ----------------------------------------------------------------------------
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  static float lastX = 0;
  static float lastY = 0;
  static float firstMouse = true;

  if(firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float yoffset = lastY - ypos; 
  float xoffset = xpos - lastX;

  lastX = xpos;
  lastY = ypos;

  if((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) &&
     ((m_drawGui == false) || (ImGui::GetIO().WantCaptureMouse == false)))
  {
    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

// ----------------------------------------------------------------------------
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

// ----------------------------------------------------------------------------
static void gui_draw()
{
  if(m_drawGui)
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");

    // ...
    ImGui::SliderFloat("X", &(cubePosition.x), -10.0f, 10.0f);
    ImGui::SliderFloat("Y", &(cubePosition.y), -10.0f, 10.0f);
    ImGui::SliderFloat("Z", &(cubePosition.z), -10.0f, 10.0f);
    ImGui::SliderFloat("Scale", &scale_ratio,    0.0f, 10.0f);
    ImGui::SliderFloat("Rotation", &rotation_angle, -180.0f, 180.0f);

    // ...
    ImGui::Checkbox("X rotate", &x_rotate); ImGui::SameLine();
    ImGui::Checkbox("Y rotate", &y_rotate); ImGui::SameLine();
    ImGui::Checkbox("Z rotate", &z_rotate); 

    // ...
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 
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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

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
  myCube.init(
    "../shader/cubeShader.vs", 
    "../shader/cubeShader.fs",
    "../resource/container.jpg"
  );

  myPlane.init(
    "../shader/planeShader.vs",
    "../shader/planeShader.fs",
    30
  );

  // ...
  glEnable(GL_DEPTH_TEST);

  // ...
  camera.Yaw = -45.0f;
  camera.Pitch = -25.0f;
  camera.ProcessMouseMovement(0, 0);

  // ...
  rotation_angle = 0.0;

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
