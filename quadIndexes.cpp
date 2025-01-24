#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>

int gScreenWidth = 640;
int gScreenHeight = 480;
// Vertex and fragment shaders
const char *vertexShaderSource;
const char *fragmentShaderSource;

// Function to initialize SDL and OpenGL

static void GLClearAllErrors(){
    while(glGetError() != GL_NO_ERROR){
        // Clears all previous OpenGL errors
    }
}

static bool GLCheckErrorStatus(const char* file, const char* function, int line) {
    GLenum error;
    bool errorOccurred = false;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error at " << file << ":" << line << " in function '" << function << "' - Error Code: " << error << std::endl;
        errorOccurred = true;
    }
    return errorOccurred;
}

// Macro to clear errors, execute OpenGL function, and check for errors
#define GLCheck(x) GLClearAllErrors(); x; if (GLCheckErrorStatus(__FILE__, __func__, __LINE__)) { std::cerr << "OpenGL error occurred!" << std::endl; }

SDL_Window *initSDLAndOpenGL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window *window = SDL_CreateWindow("OpenGL Triangle",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        exit(-1);
    }

    return window;
}

// Function to check for shader compile errors
void checkCompileErrors(GLuint shader, const std::string &type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n";
        }
    }
}

std::string readShaderFile(const std::string &filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open())
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        exit(-1);
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

// Function to compile shaders
GLuint compileShader(GLenum type, const char *source)
{
    std::string shaderSource = readShaderFile(source);
    const char *shaderSourceCStr = shaderSource.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSourceCStr, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

// Function to create a shader program
GLuint createShaderProgram()
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "quad_vertex_shader.glsl");
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "quad_fragment_shader.glsl");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLuint setupVertexData()
{
    float vertices[] = {
        // Positions         // Colors
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // Top-left vertex (Red)
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // Top-right vertex (Green)
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left vertex (Blue)
        0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // Bottom-right vertex (Yellow)
    };

    GLuint indices[] = {
        0, 1, 2,  // First triangle: Top-left, Top-right, Bottom-left
        1, 3, 2   // Second triangle: Top-right, Bottom-right, Bottom-left
    };

    GLuint VAO, VBO, EBO, VBO2;
    GLCheck(glGenVertexArrays(1, &VAO));
    GLCheck(glBindVertexArray(VAO));

    glGenBuffers(1, &VBO);
    GLCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind VBO (not strictly required)
    glBindVertexArray(1);              // Unbind VAO (not strictly required)

    return VAO;
}

// Main loop function
void mainLoop(SDL_Window *window, GLuint shaderProgram, GLuint VAO)
{
    bool quit = false;
    SDL_Event e;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Use EBO indices
        glBindVertexArray(0);

        // Swap the window
        SDL_GL_SwapWindow(window);
    }
}

// Function to clean up OpenGL and SDL
void cleanup(SDL_Window *window, GLuint VAO, GLuint shaderProgram)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    // Initialize SDL and OpenGL
    SDL_Window *window = initSDLAndOpenGL();

    // Set up shaders and vertex data
    GLuint VAO = setupVertexData();
    GLuint shaderProgram = createShaderProgram();

    // Run the main loop
    mainLoop(window, shaderProgram, VAO);

    // Clean up resources
    cleanup(window, VAO, shaderProgram);

    return 0;
}

// g++ -o opengl_program openGlMain.cpp -lSDL2 -lGLEW -lGL -I/usr/include/SDL2