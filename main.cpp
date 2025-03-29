#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Compute ShaderԴ����
const char* computeShaderSource = R"(
#version 430
layout(local_size_x = 32) in;

layout(std430, binding = 0) buffer Data {
    float data[];
};

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx < data.length()) {
        data[idx] = data[idx] * 2.0 + 1.0; // ���Լ��㣺2x + 1
    }
}
)";

int main() {
    // ��ʼ��GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // ����OpenGL������
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������ش��ڣ�Compute Shader����Ҫ�ɼ����ڣ�
    GLFWwindow* window = glfwCreateWindow(1, 1, "Compute Demo", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ����Compute Shader
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    glCompileShader(computeShader);

    // �����ɫ���������
    GLint success;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
        std::cerr << "Compute Shader�������:\n" << infoLog << std::endl;
    }

    // ������������
    GLuint computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    // ������Ӵ���
    glGetProgramiv(computeProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(computeProgram, 512, NULL, infoLog);
        std::cerr << "�������Ӵ���:\n" << infoLog << std::endl;
    }

    // ׼����������
    constexpr int DATA_SIZE = 8; // С������������֤
    float initialData[DATA_SIZE] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };

    // ����SSBO
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(initialData), initialData, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // ִ�м�����ɫ��
    glUseProgram(computeProgram);
    const int WORKGROUP_SIZE = 32;
    int groupCount = (DATA_SIZE + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
    glDispatchCompute(groupCount, 1, 1);

    // ȷ���������
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // ��ȡ���
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    float* result = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    // ��ӡ��������Ա�
    std::cout << "��������֤ (2x + 1):\n";
    for (int i = 0; i < DATA_SIZE; ++i) {
        std::cout << initialData[i] << " -> " << result[i] << std::endl;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // ������Դ
    glDeleteBuffers(1, &ssbo);
    glDeleteProgram(computeProgram);
    glDeleteShader(computeShader);

    glfwTerminate();
    return 0;
}
