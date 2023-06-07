#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "light.hpp"

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

const float MIXVALUE = 0.0;
const vec4 CLEARCOLOR = vec4(0.1f, 0.1f, 0.1f, 1.0f);
//const float pi = 3.14;

float deltaTime = 0.0f; 
float lastFrame = 0.0f; 

bool is_F_Pressed = false;
bool is_P_Pressed = false;
bool enable_mouse_camera = false;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;  // mouse position

vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
Camera camera(cameraPos);

vec4 clear_color = CLEARCOLOR;

/* lighting! */

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

float vertices[] = {
    // Position             // TexCod       // Color                // Normal
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,   -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.5f,  0.31f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.5f,  0.31f,    0.0f,  1.0f,  0.0f
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

    // light
    DirLight dirLight(vec3(-0.2f, -1.0f, -0.3f), vec3(0.945f, 0.549f, 0.153f), 0.1f, 0.3f, 0.8f);
    PointLight pointLight(vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 0.2f, 0.5f, 1.0f, 1.0f, 0.22f, 0.20f);
    float cutOff = 12.5, outercutOff = 17.5;
    SpotLight spotLight(camera.Position, camera.Front, vec3(0.9f, 0.9f, 1.0f), cos(radians(cutOff)), cos(radians(outercutOff)), 0.2f, 0.5f, 1.0f, 1.0f, 0.045f, 0.0075f);

    Material cubeMaterial = {
        vec3(0.135f, 0.2225f, 0.1575f),
        vec3(0.54f, 0.89f, 0.63f),
        vec3(0.316228f, 0.316228f, 0.316228f),
        0.1 * 128
    };

    Material boxMaterial = {
        vec3(0.05f, 0.05f, 0.05f),
        vec3(0.50f, 0.50f, 0.50f),
        vec3(0.70f, 0.70f, 0.70f),
        0.25 * 128
    };

    // boxPosOffset
    vec3 boxPosOffset[] = {
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
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Cube
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Box
    unsigned int boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // light Source
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Texture
    Texture tex_container("../assets/texture/container2.png", PNG);
    Texture tex_container_specular("../assets/texture/container2_specular.png", PNG);
    Texture tex_avatar("../assets/texture/avatar.png", PNG);
    Texture tex_matrix("../assets/texture/matrix.jpg", JPG);
    float mixvalue = MIXVALUE;

    // Shaders
    Shader cube_shader("cube_shader.vert", "cube_shader.frag");
    Shader box_shader("box_shader.vert", "box_shader.frag");
    Shader light_shader("light_shader.vert", "light_shader.frag");

    box_shader.use();
    box_shader.setInt("material.diffuse", 0);
    box_shader.setInt("material.specular", 1);
    box_shader.setInt("avatarTexture", 2);
    box_shader.setInt("matrixTexture", 3);

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
            ImGui::Begin("config");                                                   

            ImGui::Text("This is my first OpenGL project!");
            ImGui::Text("Global setting:");
            ImGui::Checkbox("enable_mouse_camera (key_M/N)", &enable_mouse_camera);   
            ImGui::Checkbox("firstMouse", &firstMouse);
            ImGui::Checkbox("camera.isFPScamera (Key_F)", &camera.isFPScamera);
            ImGui::Checkbox("camera.constrainPitch (Key_P)", &camera.constrainPitch);
            ImGui::ColorEdit4("clear color", (float*)&clear_color);         
            ImGui::Text("Box config:");
            ImGui::SliderFloat("boxTex_mixvalue", &mixvalue, 0.0f, 1.0f);             
            ImGui::Text("CubeMaterial config:");                                      
            ImGui::ColorEdit3("ambient", (float*)&cubeMaterial.ambient);
            ImGui::ColorEdit3("diffuse", (float*)&cubeMaterial.diffuse);
            ImGui::ColorEdit3("specular", (float*)&cubeMaterial.specular);
            ImGui::SliderFloat("shininess", &cubeMaterial.shininess, 0.0001f, 256.0000f);
            ImGui::Text("dirLight config:");
            ImGui::ColorEdit3("dirLight color", (float*)&dirLight.color);
            ImGui::Text("pointLight config:");
            ImGui::ColorEdit3("pointLight color", (float*)&pointLight.color);
            ImGui::SliderFloat("ambient", &pointLight.a, 0.0000f, 1.0000f);
            ImGui::SliderFloat("diffuse", &pointLight.d, 0.0000f, 1.0000f);
            ImGui::SliderFloat("specular", &pointLight.s, 0.0000f, 1.0000f);


            if (ImGui::Button("reset"))                                               
            {
                enable_mouse_camera = false;
                firstMouse = true;
                mixvalue = MIXVALUE;
                clear_color = CLEARCOLOR;
                camera.resetcamera();
            }
            //ImGui::SameLine();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        ImGui::Render();

        pointLight.updateLight();
        dirLight.updateLight();
        spotLight.updateLight(camera.Position, camera.Front);

        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // movable light position
        vec3 lightPos(1.0f);
        lightPos.x = pointLight.position.x + cos(glfwGetTime() / 2) * 2.0f;
        lightPos.y = pointLight.position.y + sin(glfwGetTime() / 2) * 2.0f;
        lightPos.z = pointLight.position.z;

        
        // mvp
        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);
        mat3 normalMatrix = mat3(1.0f);
        view = camera.getViewMatrix();
        projection = perspective(radians(camera.Fov), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
        
        /* draw cube */
        glBindVertexArray(cubeVAO);
        cube_shader.use();

        model = mat4(1.0f);
        model = rotate(model, (float)(glfwGetTime() * radians(5.0)), vec3(0.5f, 0.3f, 0.5f));
        normalMatrix = mat3(transpose(inverse(model)));
        cube_shader.setMat3("normalMatrix", &normalMatrix);
        cube_shader.setMat4("model", &model);
        cube_shader.setMat4("view", &view);
        cube_shader.setMat4("projection", &projection);
        cube_shader.setVec3("viewPos", &camera.Position);

        cube_shader.setVec3("dirlight.direction",       &dirLight.direction);
        cube_shader.setVec3("dirlight.ambient",         &dirLight.ambient);
        cube_shader.setVec3("dirlight.diffuse",         &dirLight.diffuse);
        cube_shader.setVec3("dirlight.specular",        &dirLight.specular);

        cube_shader.setVec3("pointlight.position",      &lightPos);
        cube_shader.setVec3("pointlight.ambient",       &pointLight.ambient);
        cube_shader.setVec3("pointlight.diffuse",       &pointLight.diffuse);
        cube_shader.setVec3("pointlight.specular",      &pointLight.specular);
        cube_shader.setFloat("pointlight.constant",     pointLight.constant);
        cube_shader.setFloat("pointlight.linear",       pointLight.linear);
        cube_shader.setFloat("pointlight.quadratic",    pointLight.quadratic);

        cube_shader.setVec3("spotlight.position",       &spotLight.position);
        cube_shader.setVec3("spotlight.direction",      &spotLight.direction);
        cube_shader.setVec3("spotlight.ambient",        &spotLight.ambient);
        cube_shader.setVec3("spotlight.diffuse",        &spotLight.diffuse);
        cube_shader.setVec3("spotlight.specular",       &spotLight.specular);
        cube_shader.setFloat("spotlight.cutOff",        spotLight.cutOff);
        cube_shader.setFloat("spotlight.outerCutOff",   spotLight.outerCutOff);
        cube_shader.setFloat("spotlight.constant",      spotLight.constant);
        cube_shader.setFloat("spotlight.linear",        spotLight.linear);
        cube_shader.setFloat("spotlight.quadratic",     spotLight.quadratic);

        cube_shader.setVec3("material.ambient",         &cubeMaterial.ambient);
        cube_shader.setVec3("material.diffuse",         &cubeMaterial.diffuse);
        cube_shader.setVec3("material.specular",        &cubeMaterial.specular);
        cube_shader.setFloat("material.shininess",      cubeMaterial.shininess);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* draw box */
        glBindVertexArray(boxVAO);
        box_shader.use();
        tex_container.use(0);
        tex_container_specular.use(1);
        tex_avatar.use(2);
        tex_matrix.use(3);

        for (unsigned i = 0; i < 9; i++)
        {
            model = mat4(1.0f);
            float angle = 5.0f * i + 20.0f;
            model = translate(model, boxPosOffset[i]);
            model = rotate(model, (float)(glfwGetTime() * radians(angle)), vec3(1.0f, 0.3f, 0.5f));
            normalMatrix = mat3(transpose(inverse(model)));
            
            box_shader.setMat3("normalMatrix", &normalMatrix);
            box_shader.setMat4("model", &model);
            box_shader.setMat4("view", &view);
            box_shader.setMat4("projection", &projection);
            box_shader.setVec3("viewPos", &camera.Position);
            box_shader.setFloat("time", (float)(glfwGetTime()));
            box_shader.setFloat("mixvalue", mixvalue);

            box_shader.setVec3("dirlight.direction",        &dirLight.direction);
            box_shader.setVec3("dirlight.ambient",          &dirLight.ambient);
            box_shader.setVec3("dirlight.diffuse",          &dirLight.diffuse);
            box_shader.setVec3("dirlight.specular",         &dirLight.specular);

            box_shader.setVec3("pointlight.position",       &lightPos);
            box_shader.setVec3("pointlight.ambient",        &pointLight.ambient);
            box_shader.setVec3("pointlight.diffuse",        &pointLight.diffuse);
            box_shader.setVec3("pointlight.specular",       &pointLight.specular);
            box_shader.setFloat("pointlight.constant",      pointLight.constant);
            box_shader.setFloat("pointlight.linear",        pointLight.linear);
            box_shader.setFloat("pointlight.quadratic",     pointLight.quadratic);

            box_shader.setVec3("spotlight.position",       &spotLight.position);
            box_shader.setVec3("spotlight.direction",      &spotLight.direction);
            box_shader.setVec3("spotlight.ambient",        &spotLight.ambient);
            box_shader.setVec3("spotlight.diffuse",        &spotLight.diffuse);
            box_shader.setVec3("spotlight.specular",       &spotLight.specular);
            box_shader.setFloat("spotlight.cutOff",        spotLight.cutOff);
            box_shader.setFloat("spotlight.outerCutOff",   spotLight.outerCutOff);
            box_shader.setFloat("spotlight.constant",      spotLight.constant);
            box_shader.setFloat("spotlight.linear",        spotLight.linear);
            box_shader.setFloat("spotlight.quadratic",     spotLight.quadratic);

            box_shader.setVec3("material.ambient",          &boxMaterial.ambient);
            box_shader.setVec3("material.diffuse",          &boxMaterial.diffuse);
            box_shader.setVec3("material.specular",         &boxMaterial.specular);
            box_shader.setFloat("material.shininess",       boxMaterial.shininess);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        /* draw light sourse: light cube */
        glBindVertexArray(lightVAO);
        light_shader.use();

        model = mat4(1.0f);
        model = translate(model, lightPos);
        model = scale(model, vec3(0.2f));
        light_shader.setMat4("model", &model);
        light_shader.setMat4("view", &view);
        light_shader.setMat4("projection", &projection);
        light_shader.setVec3("lightColor", &pointLight.color);
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
    glDeleteVertexArrays(1, &boxVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &cubeVBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

