#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace glm;
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//const float pi = 3.14;

float deltaTime = 0.0f; 
float lastFrame = 0.0f; 

bool is_F_Pressed = false;
bool is_P_Pressed = false;
bool enable_mouse_camera = false;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;  // mouse position

Camera camera(vec3(0.0f, 0.0f, 3.0f));

vec4 clear_color(0.1f, 0.1f, 0.1f, 1.0f);

// lighting!
vec3 lightPos(1.2f, 1.0f, 2.0f);
vec3 lightColor(1.0f, 1.0f, 1.0f);



float vertices[] = {
    // Position             // TexCod       //Color
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
                                                         
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
                                                         
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
                                                         
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
                                                         
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
                                                         
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f,  0.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveCamera(deltaTime, FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveCamera(deltaTime, BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveCamera(deltaTime, LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveCamera(deltaTime, RIGHT);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.moveCamera(deltaTime, UPWARD);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.moveCamera(deltaTime, DOWN);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !is_F_Pressed)      // Press key F to enable/disable the FPS camera mode
    {
        camera.setFPScamera();
        is_F_Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        is_F_Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !is_P_Pressed)      // Press key P to enable/disable the constrainPitch
    {
        camera.setconstrainPitch();
        is_P_Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
        is_P_Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)                       // Press key M to enable the mouse_camera
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        enable_mouse_camera = true;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                       // Press key N to disable the mouse_camera
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        firstMouse = true;
        enable_mouse_camera = false;
    }

}

void mouse_callback(GLFWwindow* window, double input_xpos, double input_ypos) {

    float xpos = static_cast<float>(input_xpos);
    float ypos = static_cast<float>(input_ypos);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if (enable_mouse_camera) 
    { 
        camera.rotateCamera(xoffset, yoffset); 
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.zoomCamera(static_cast<float>(yoffset));
}

int main() {

    // glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // imGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui::StyleColorsDark(); // Alternative: ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // Gl features
    glEnable(GL_DEPTH_TEST);
    
    /* Vertex Input */
    // Cube
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light Source
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Texture
    Texture tex_container("../assets/texture/container.jpg", 0);
    Texture tex_avatar("../assets/texture/avatar.png", 1);
    float mixvalue = 0.5;

    // Shaders
    Shader box_shader("box_shader.vert", "box_shader.frag");
    Shader light_shader("light_shader.vert", "light_shader.frag");

    box_shader.use();
    box_shader.setInt("containerTexture", 0);
    box_shader.setInt("avatarTexture", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("config");                                                     // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is my first OpenGL project!");                            // Display some text (you can use a format strings too)
            ImGui::Checkbox("enable_mouse_camera (key_M/N)", &enable_mouse_camera);     // Edit bools storing our window open/close state
            ImGui::Checkbox("firstMouse", &firstMouse);
            ImGui::Checkbox("camera.isFPScamera (Key_F)", &camera.isFPScamera);
            ImGui::Checkbox("camera.constrainPitch (Key_P)", &camera.constrainPitch);
            ImGui::SliderFloat("mixvalue", &mixvalue, 0.0f, 1.0f);                      // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit4("clear color", (float*)&clear_color);                     // Edit 4 floats representing a color
            ImGui::ColorEdit4("light cube color", (float*)&lightColor);                     // 

            if (ImGui::Button("reset"))                                                 // Buttons return true when clicked (most widgets return true when edited/activated)
            {
                enable_mouse_camera = false;
                firstMouse = true;
                mixvalue = 0.5;
                clear_color = vec4(0.1f, 0.1f, 0.1f, 1.0f);
                camera.resetcamera();
            }
            //ImGui::SameLine();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        ImGui::Render();

        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        tex_container.use0();
        tex_avatar.use1();

        // mvp
        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);
        view = camera.getViewMatrix();
        projection = perspective(radians(camera.Fov), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

        /* draw box */
        glBindVertexArray(cubeVAO);
        box_shader.use();

        box_shader.setFloat("mixvalue", mixvalue);
        model = rotate(model, (float)(glfwGetTime() * radians(15.0)), glm::vec3(1.0f, 0.3f, 0.5f));
        box_shader.setMat4("model", &model);
        box_shader.setMat4("view", &view);
        box_shader.setMat4("projection", &projection);
        box_shader.setVec3("lightColor", &lightColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* draw light cube */
        glBindVertexArray(lightVAO);
        light_shader.use();

        model = mat4(1.0f);
        model = translate(model, lightPos);
        model = scale(model, vec3(0.2f));
        light_shader.setMat4("model", &model);
        light_shader.setMat4("view", &view);
        light_shader.setMat4("projection", &projection);
        light_shader.setVec3("lightColor", &lightColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float> (glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &cubeVBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

