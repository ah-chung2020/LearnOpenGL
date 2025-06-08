#include "Base.h"

#include <iostream>
#include "shader/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint texture;

glm::mat4 transform(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 orthographicMatrix(1.0f);
glm::mat4 perspectiveMatrix(1.0f);

void prepareTexture() {

    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load("assets/textures/xiaomao.jpg", &width, &height, &channels, STBI_rgb_alpha);

    // 生成纹理对象
    glGenTextures(1, &texture);

    // 激活纹理单元0
    //（先激活纹理单元，后面绑定的纹理对象就会和这个激活的纹理单元相连接）
    glActiveTexture(GL_TEXTURE0);

    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, texture);

    // 传输纹理数据，开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    // 设置纹理过滤方式
    // 当需要的像素 > 纹理的像素，采用双线性插值的方式过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 当需要的像素 < 纹理的像素，采用临近插值的方式过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 设置纹理包裹方式 Wrapping
    // U方向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // V方向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void prepareModelingTransform() {
    transform = glm::identity<glm::mat4>();
    transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
}

void prepareCameraTransform() {

    glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -2.0f);
    glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

    viewMatrix = glm::lookAt(eyePosition, targetPosition, upDirection);
}

void prepareOrthographic() {

    orthographicMatrix = 
        glm::ortho(-2.0f, 2.0f,
                -2.0f, 2.0f,
                 2.0f, -2.0f
    );
}

void preparePerspectiveMatrix() {

    perspectiveMatrix = glm::perspective(
        glm::radians(60.0f),
        static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
        0.1f,
        1000.0f
    );
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
      
    const char* name = glfwGetKeyName(key, scancode);
    std::cout << "name: " << name << ", scancode: " << scancode << ", action: " << action << std::endl;


}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        printf("左键按下\n");
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        printf("左键抬起\n");
    }
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    printf("鼠标坐标: (%.1f, %.1f)\n", xpos, ypos);
}


int main()
{

    //// NDC coords testing
    //glm::vec3 point1 = glm::vec3(0.5f, -0.5f, 0.0f);
    //glm::vec3 point2 = glm::vec3(-0.5f, -0.5f, 0.0f);
    //glm::vec3 point3 = glm::vec3(0.0f, 0.5f, 0.0f);

    //glm::mat4 myViewMatrix = glm::lookAt(
    //    glm::vec3(0.0f, 0.0f, -2.0f), 
    //    glm::vec3(0.0f, 0.0f, 0.0f),
    //    glm::vec3(0.0f, 1.0f, 0.0f)
    //);

    //glm::vec3 point1_output = myViewMatrix * glm::vec4(point1, 1.0f);

    //std::cout << "point1: " << glm::to_string(point1) << std::endl;
    //std::cout << "point1_output: " << glm::to_string(point1_output) << std::endl;


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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader _shaderProgram = Shader(
        "assets/shaders/triangle.vert",
        "assets/shaders/triangle.frag"
    );

    float vertices[] = {
        // positions         // colors         // uv
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f   // top 

    };
    /*
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top 

    };
    */
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
        8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    prepareTexture();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        //processInput(window);

        // render
        // ------
        _shaderProgram.Begin();

        prepareModelingTransform();
        prepareCameraTransform();
        prepareOrthographic();
        preparePerspectiveMatrix();

        // 0号纹理单元
        _shaderProgram.SetInt("sampler", 0);
        _shaderProgram.SetMat4("transform", transform);
        _shaderProgram.SetMat4("viewMatrix", viewMatrix);
        _shaderProgram.SetMat4("projectionMatrix", perspectiveMatrix);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        _shaderProgram.End();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
