#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int keyCode, int scanCode, int action, int mod);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void prepare() {
    
    /*
     Single Buffer
    */

    float vertices[] = {
        -0.5, -0.5, 0.0,            // 顶点1
        0.5, -0.5, 0.0,             // 顶点2
        0.0, 0.5, 0.0,              // 顶顶3
    };

    float colors[] = {
        1.0, 0.0, 0.0,              // 顶点颜色1
        0.0, 1.0, 0.0,              // 顶点颜色2
        0.0, 0.0, 1.0,              // 顶点颜色3
    };

    GLuint vertexVBO, colorVBO;

    glGenBuffers(1, &vertexVBO);                    // 生成vbo
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);       // 绑定vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 塞入数据

    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    GLuint singleBufferVAO;
    glGenBuffers(1, &singleBufferVAO);          // 生成vao
    glBindVertexArray(singleBufferVAO);         // 绑定

    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                         // VAO中0号位置
        3,                         // 一个属性有3个元素
        GL_FLOAT,                  // 数据是 float 类型的
        GL_FALSE,                   // 是否归一化
        3 * sizeof(float),          // 步长
        (void*)0                    // 单元内偏移
    );

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    prepare();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    std::cout << "frame buffer size callback." << std::endl;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int keyCode, int scanCode, int action, int mod) {
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    if(keyCode == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, true);
}