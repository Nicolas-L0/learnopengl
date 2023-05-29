#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

using namespace glm;
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//const float pi = 3.14;

float deltaTime = 0.0f; 
float lastFrame = 0.0f; 

bool enable_mouse_camera = false;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;  // mouse position

Camera camera(vec3(0.0f, 0.0f, 3.0f));

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
                                                    
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
                                                    
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
                                                    
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
                                                    
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
                                                    
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f
};

vec3 cubePositions[] = {
    vec3(0.0f,  0.0f,  0.0f),
    vec3(2.0f,  5.0f, -15.0f),
    vec3(-1.5f, -2.2f, -2.5f),
    vec3(-3.8f, -2.0f, -12.3f),
    vec3(2.4f, -0.4f, -3.5f),
    vec3(-1.7f,  3.0f, -7.5f),
    vec3(1.3f, -2.0f, -2.5f),
    vec3(1.5f,  2.0f, -2.5f),
    vec3(1.5f,  0.2f, -1.5f),
    vec3(-1.3f,  1.0f, -1.5f)
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)                       // Press key F to enable/disable the FPS camera mode
        camera.setFPScamera();
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)                       // Press key P to enable/disable the constrainPitch
        camera.setconstrainPitch();
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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


    // Gl features
    glEnable(GL_DEPTH_TEST);
    
    // Vertex Input
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Texture
    Texture tex_container("../assets/texture/container.jpg", 0);
    Texture tex_avatar("../assets/texture/avatar.png", 1);

    // Shaders
    Shader box_shader("shader.vert", "shader.frag");

    box_shader.use();
    box_shader.setInt("containerTexture", 0);
    box_shader.setInt("avatarTexture", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        float timeValue = static_cast<float>(glfwGetTime());

        tex_container.use0();
        tex_avatar.use1();
        glBindVertexArray(VAO);
        box_shader.use();

        // mvp
        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);

        //view = translate(view, vec3(0.0f, 0.0f, -3.0f));
        //view = lookAt(vec3(sin(glfwGetTime()) * 10.0f, 0.0f, cos(glfwGetTime()) * 10.0f), cameraTarget, cameraUp);    camera rotating
        view = camera.getViewMatrix();
        projection = perspective(radians(camera.Fov), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

        box_shader.setMat4("view", &view);
        box_shader.setMat4("projection", &projection);

        for (unsigned int i = 0; i<10; i++)
        {
            mat4 model = mat4(1.0f);
            float angle = 20.0f * i + 20.0f;
            model = translate(model, cubePositions[i]);
            model = rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = translate(model, vec3(0.1f * i, 0.0f, 0.0f)); //For testing the impact of different orderings of rotate & translate
            box_shader.setMat4("model", &model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float> (glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //cout << deltaTime << endl;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

