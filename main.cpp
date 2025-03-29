#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

// 粒子结构体（需与着色器中的定义完全一致）
struct Particle {
    glm::vec4 position; // xyz: 位置, w: 生命周期
    glm::vec4 velocity; // xyz: 速度, w: 存活时间
};

// 编译着色器的工具函数
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // 检查编译错误
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader编译失败:\n" << infoLog << std::endl;
    }
    return shader;
}

int main() {
    // 初始化 GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "Compute Shader particle", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // 定义粒子数量（必须为 256 的倍数）
    constexpr uint32_t PARTICLE_COUNT = 1024;

    // 初始化粒子数据（随机位置和速度）
    std::vector<Particle> particles(PARTICLE_COUNT);
    for (auto& p : particles) {
        p.position = glm::vec4(
            (rand() / (float)RAND_MAX - 0.5f) * 2.0f, // x: [-1, 1]
            (rand() / (float)RAND_MAX - 0.5f) * 2.0f, // y: [-1, 1]
            (rand() / (float)RAND_MAX - 0.5f) * 2.0f, // z: [-1, 1]
            1.0f // 生命周期
        );
        p.velocity = glm::vec4(
            0.0f, 0.0f, 0.0f, // 初始速度为0
            20.0 // 存活时间: 1~6秒
        );
    }

    // ========== OpenGL 对象初始化 ==========
    // 1. 创建 SSBO（粒子数据）
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, PARTICLE_COUNT * sizeof(Particle), particles.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // 2. 创建 Compute Shader 程序
    const char* computeShaderSource = R"glsl(
        #version 430 core
        layout(local_size_x = 256) in;

        struct Particle {
            vec4 position; // xyz: 位置, w: 生命周期
            vec4 velocity; // xyz: 速度, w: 存活时间
        };

        layout(binding = 0) buffer Particles {
            Particle particles[];
        };
        uniform float deltaTime;
        uniform vec3 gravity = vec3(0.0, -9.8, 0.0);

        void main() {
            uint idx = gl_GlobalInvocationID.x;
            if (particles[idx].velocity.w > 0.0) {
                particles[idx].velocity.xyz += gravity * deltaTime;
                particles[idx].position.xyz += particles[idx].velocity.xyz * deltaTime;
                particles[idx].velocity.w -= deltaTime;
            }

            // 计算着色器修改
            float floorHeight = -1.4; // 地面高度
            float restitution = 0.8;  // 反弹系数（0.8保留80%速度）

            if (particles[idx].position.y < floorHeight) {
                particles[idx].position.y = floorHeight;                    // 防止穿透
                particles[idx].velocity.y *= -restitution;                  // Y轴速度反转并衰减
                particles[idx].velocity.xz *= 0.9;                         // 水平方向摩擦力
            }

        }
    )glsl";
    GLuint computeShader = compileShader(GL_COMPUTE_SHADER, computeShaderSource);
    GLuint computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    // 3. 创建渲染着色器程序
    const char* vertexShaderSource = R"glsl(
        #version 430 core

        struct Particle {
            vec4 position;
            vec4 velocity;
        };

        layout(binding = 0) buffer Particles { Particle particles[]; };
        uniform mat4 viewProj;
        void main() {
            vec3 pos = particles[gl_VertexID].position.xyz;
            gl_Position = viewProj * vec4(pos, 1.0);
            gl_PointSize = 5.0;
        }
    )glsl";
    const char* fragmentShaderSource = R"glsl(
        #version 430 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )glsl";
    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint renderProgram = glCreateProgram();
    glAttachShader(renderProgram, vertShader);
    glAttachShader(renderProgram, fragShader);
    glLinkProgram(renderProgram);

    // 4. 创建 VAO（OpenGL 4.3 核心模式需要）
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 主循环
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // 计算时间差
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 处理输入
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // 更新粒子（Compute Shader）
        glUseProgram(computeProgram);
        glUniform1f(glGetUniformLocation(computeProgram, "deltaTime"), deltaTime);
        glDispatchCompute(PARTICLE_COUNT / 256, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // 渲染粒子
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(renderProgram);

        // 简单视图矩阵（正投影）
        glm::mat4 viewProj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -10.0f, 10.0f);
        glUniformMatrix4fv(glGetUniformLocation(renderProgram, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProj));

        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

        glfwSwapBuffers(window);
    }

    // 清理资源
    glDeleteProgram(computeProgram);
    glDeleteProgram(renderProgram);
    glDeleteBuffers(1, &ssbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    return 0;
}
