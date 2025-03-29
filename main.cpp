#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Compute Shader源代码
const char* computeShaderSource = R"(
#version 430
layout(local_size_x = 32) in;

layout(std430, binding = 0) buffer Data {
    float data[];
};

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx < data.length()) {
        data[idx] = data[idx] * 2.0 + 1.0; // 测试计算：2x + 1
    }
}
)";

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 配置OpenGL上下文
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建隐藏窗口（Compute Shader不需要可见窗口）
    GLFWwindow* window = glfwCreateWindow(1, 1, "Compute Demo", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 创建Compute Shader
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    glCompileShader(computeShader);

    // 检查着色器编译错误
    GLint success;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
        std::cerr << "Compute Shader编译错误:\n" << infoLog << std::endl;
    }

    // 创建程序并链接
    GLuint computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    // 检查链接错误
    glGetProgramiv(computeProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(computeProgram, 512, NULL, infoLog);
        std::cerr << "程序链接错误:\n" << infoLog << std::endl;
    }

    // 准备测试数据
    constexpr int DATA_SIZE = 8; // 小数据量便于验证
    float initialData[DATA_SIZE] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };

    // 创建SSBO
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(initialData), initialData, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // 执行计算着色器
    glUseProgram(computeProgram);
    const int WORKGROUP_SIZE = 32;
    int groupCount = (DATA_SIZE + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
    glDispatchCompute(groupCount, 1, 1);

    // 确保计算完成
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 读取结果
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    float* result = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    // 打印输入输出对比
    std::cout << "计算结果验证 (2x + 1):\n";
    for (int i = 0; i < DATA_SIZE; ++i) {
        std::cout << initialData[i] << " -> " << result[i] << std::endl;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // 清理资源
    glDeleteBuffers(1, &ssbo);
    glDeleteProgram(computeProgram);
    glDeleteShader(computeShader);

    glfwTerminate();
    return 0;
}
