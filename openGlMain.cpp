#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex and fragment shaders
const char* vertexShaderSource = R"(
    #version 330 core
    uniform mat4 model;
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec3 aCol;
    out vec3 fragColor;
    void main()
    {
        gl_Position = model * vec4(aPos, 0.0, 1.0);
        fragColor = aCol;
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    in vec3 fragColor; // Receive color from vertex shader
    out vec4 FragColor;
    void main() {
        FragColor = vec4(fragColor, 1.0); // Use interpolated color
    }
)";


// Function to initialize SDL and OpenGL
SDL_Window* initSDLAndOpenGL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow("OpenGL Triangle",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        exit(-1);
    }

    return window;
}

// Function to check for shader compile errors
void checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

// Function to compile shaders
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

// Function to create a shader program
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLuint setupVertexData() {
    float vertices[] = {
        // Positions      // Colors
         0.0f,  0.5f,      1.0f, 0.0f, 0.0f, // Top vertex (Red)
        -0.5f, -0.5f,      0.0f, 1.0f, 0.0f, // Bottom-left vertex (Green)
         0.5f, -0.5f,      0.0f, 0.0f, 1.0f, // Bottom-right vertex (Blue)
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

// Main loop function
void mainLoop(SDL_Window* window, GLuint shaderProgram, GLuint VAO) {
    bool quit = false;
    SDL_Event e;

    // Initialize scaling variables
    float scaleFactor = 1.0f;
    bool scalingUp = true;
    float scalingSpeed = 0.01f;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    if (modelLoc == -1) {
        std::cerr << "Error finding uniform location for 'model'" << std::endl;
        exit(-1);
    }

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update scaling factor
        if (scalingUp) {
            scaleFactor += scalingSpeed;
            if (scaleFactor >= 2.0f) scalingUp = false;
        } else {
            scaleFactor -= scalingSpeed;
            if (scaleFactor <= 0.1f) scalingUp = true;
        }

        // Create scaling matrix
        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, 1.0f));

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap the window
        SDL_GL_SwapWindow(window);
    }
}

// Function to clean up OpenGL and SDL
void cleanup(SDL_Window* window, GLuint VAO, GLuint shaderProgram) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    // Initialize SDL and OpenGL
    SDL_Window* window = initSDLAndOpenGL();

    // Set up shaders and vertex data
    GLuint shaderProgram = createShaderProgram();
    GLuint VAO = setupVertexData();

    // Run the main loop
    mainLoop(window, shaderProgram, VAO);

    // Clean up resources
    cleanup(window, VAO, shaderProgram);

    return 0;
}