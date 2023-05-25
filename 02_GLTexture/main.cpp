#include "shader.hpp"
#include "texture.hpp"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float pi = 3.14;

float vertices[] = {
    //Rect                  //color                 //tex 
    0.2f, 0.3f, 0.0f,       0.0f, 0.0f, 0.0f,       1.0f, 1.0f,
    0.2f, -0.3f, 0.0f,      0.0f, 0.0f, 0.0f,       1.0f, 0.0f,
    -0.9f, -0.3f, 0.0f,     0.0f, 0.0f, 0.0f,       0.0f, 0.0f,
    -0.9f,  0.3f, 0.0f,     0.0f, 0.0f, 0.0f,       0.0f, 1.0f,
    //Tri
    0.2f, 0.7f, 0.0f,       1.0f, 1.0f, 0.0f,       0.0f, 1.0f,
    0.2f, -0.7f, 0.0f,      0.0f, 1.0f, 1.0f,       0.0f, 0.0f,
    0.9f, 0.0f, 0.0f,       1.0f, 0.0f, 1.0f,       1.0f, 0.5f
};

unsigned int indices[] = {
    //Rect
    0, 1, 3,
    1, 2, 3,
    //Tri
    4, 5, 6
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
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

    // Vertex Input
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Texture
    Texture tex_container("../assets/texture/container.jpg");
    Texture tex_wall("../assets/texture/wall.jpg");

    // Shaders
    Shader rect_shader("vshader.vs", "fshaderO.fs");
    Shader tri_shader("vshader.vs", "fshaderY.fs");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        float timeValue = static_cast<float>(glfwGetTime());


        tex_container.use();
        glBindVertexArray(VAO);
        rect_shader.use();
        rect_shader.setFloat("time", timeValue);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        tex_wall.use();
        tri_shader.use();
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //float colorValue3 = (sin(2 * pi * timeValue / 3) / 2.0f) + 0.5f;
        tri_shader.setFloat("time", timeValue);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

