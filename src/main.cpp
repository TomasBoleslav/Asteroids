#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

const std::string vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
"}";

const std::string fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(0.5f, 1.0f, 0.5f, 1.0f);\n"
"}\n";

void checkShaderCompileErrors(unsigned int shader, const std::string type)
{
    const int logSize = 512;
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[logSize];
        glGetShaderInfoLog(shader, logSize, nullptr, infoLog);
        std::cout << "Failed to compile " << type << " shader:" << std::endl << infoLog << std::endl;
    }
}

unsigned int getShader(const std::string& shaderSource, unsigned int shaderType, const std::string& typeString)
{
    const char* c_str = shaderSource.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);
    checkShaderCompileErrors(shader, typeString);
    return shader;
}

void checkShaderProgramLinkingErrors(unsigned int shaderProgram)
{
    const int logSize = 512;
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[logSize];
        glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
        std::cout << "Failed to link program:" << std::endl << infoLog << std::endl;
    }
}

unsigned int getShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    unsigned int vertexShader = getShader(vertexShaderSource, GL_VERTEX_SHADER, "vertex");
    unsigned int fragmentShader = getShader(fragmentShaderSource, GL_FRAGMENT_SHADER, "fragment");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderProgramLinkingErrors(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

struct Vertex
{
    float x;
    float y;
    float z;

    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SpaceGame", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

int main()
{
    GLFWwindow* window = initWindow();
    if (!window)
    {
        return -1;
    }

    const size_t vertexCount = 4;
    const Vertex vertices[vertexCount] = {
        Vertex(0.5f, 0.5f, 0.0f),
        Vertex(0.5f, -0.5f, 0.0f),
        Vertex(-0.5f, -0.5f, 0.0f),
        Vertex(-0.5f,  0.5f, 0.0f)
    };
    const size_t indexCount = 6;
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, vertexCount, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int shaderProgram = getShaderProgram(vertexShaderSource, fragmentShaderSource);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}