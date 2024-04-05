#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using std::cout;
using std::endl;

// Simple vertex shader
const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader for triangle 1
const char *fragmentShaderSourceOne = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.6f, 0.0f, 1.0f);\n"
    "}\0";

// Fragment shader for triangle 2
const char *fragmentShaderSourceTwo = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(0.5f, 0.5f, 0.5f, 0.5f);\n"
    "}\0";

// Callback function to adjust viewport in case the GL window is modified
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Checks whether escape key is pressed to quit the process
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // Initialize a blank window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "demo", NULL, NULL);
    if (window == NULL)
    {
        cout << "Could not create window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load the OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Could not retrieve function pointers" << endl;
        return -1;
    }

    // Create VBOs for the two triangles
    unsigned int VBO[2];
    glGenBuffers(2, VBO);

    // Create VAOs for the two triangles
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    // Bind and compile the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    // Get vertex shader compilation status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
            infoLog << endl;
    }

    // Bind and compile the fragment shader for triangle one
    unsigned int fragmentShaderOne;
    fragmentShaderOne = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOne, 1, &fragmentShaderSourceOne, NULL);
    glCompileShader(fragmentShaderOne);

    // Get compilation status for fragment shader one
    glGetShaderiv(fragmentShaderOne, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShaderOne, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
            infoLog << endl;
    }

    //Bind and compile the fragment shader for triangle two
    unsigned int fragmentShaderTwo;
    fragmentShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderTwo, 1, &fragmentShaderSourceTwo, NULL);
    glCompileShader(fragmentShaderTwo);

    // Get compilation status for fragment shader two
    glGetShaderiv(fragmentShaderTwo, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShaderTwo, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
            infoLog << endl;
    }

    // Create and link a shader program for triangle one
    unsigned int shaderProgramOne;
    shaderProgramOne = glCreateProgram();
    glAttachShader(shaderProgramOne, vertexShader);
    glAttachShader(shaderProgramOne, fragmentShaderOne);
    glLinkProgram(shaderProgramOne);

    // Get link status of the shader program for triangle one
    glGetProgramiv(shaderProgramOne, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgramOne, 512, NULL, infoLog);
        cout << "ERROR::PROGRAM::LINKING_FAILED\n" <<
            infoLog << endl;
    }

    glDeleteShader(fragmentShaderOne);

    // Create and link a shader program for triangle two
    unsigned int shaderProgramTwo;
    shaderProgramTwo = glCreateProgram();
    glAttachShader(shaderProgramTwo, vertexShader);
    glAttachShader(shaderProgramTwo, fragmentShaderTwo);
    glLinkProgram(shaderProgramTwo);

    // Get link status of shader program for triangle two
    glGetProgramiv(shaderProgramTwo, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgramTwo, 512, NULL, infoLog);
        cout << "ERROR::PROGRAM::LINKING_FAILED\n" <<
            infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderTwo);

    // Vertices of triangle one
    float triangle_one[] = {
        -0.9f, -0.5f, 0.0f,
        -0.1f, -0.5f, 0.0f,
        -0.5f, 0.4f, 0.0f,
    };

    // Vertices of triangle two
    float triangle_two[] = {
        0.9f, -0.5f, 0.0f,
        0.1f, -0.5f, 0.0f,
        0.5f, 0.4f, 0.0f
    };
    
    // Initialize the VAO and VBO for triangle one
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_one), triangle_one, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Initialize the VAO and VBO for triangle two
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_two), triangle_two, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    // The actual rendering functions
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.4f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Render triangle one
        glUseProgram(shaderProgramOne);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Render triangle two
        glUseProgram(shaderProgramTwo);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
