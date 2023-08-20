#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include "CLI.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"

/* GLFW function declarations */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* Function that initialize GLFW */
void initializeGLFW();

/* Function used to initialize a window */
GLFWwindow* initializeWindow();

/* Initialize Game */
game::Game Snake;

int main(int argc, char** argv)
{

    std::string shaderPath;
    std::string imagePath;
    std::string fontsPath;

    try {
        shaderPath = CLI::checkAndGetCmdOption(argv, argc + argv, "-shader");
        imagePath = CLI::checkAndGetCmdOption(argv, argc + argv, "-image");
        fontsPath = CLI::checkAndGetCmdOption(argv, argc + argv, "-fonts");
    }
    catch (std::runtime_error& ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    /* Initialize window */
    GLFWwindow* window;

    try {
        window = initializeWindow();
    }
    catch (std::runtime_error& ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    /* OpenGL configuration */
    glViewport(0, 0, game::SCREEN_WIDTH, game::SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* initialize game */
    try {
        Snake.Init(shaderPath, imagePath, fontsPath);
    }
    catch (std::runtime_error& ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    /* deltaTime variables */
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    const float FRAMERATE = 3.0f;

    bool changedDirection = false;

    while (!glfwWindowShouldClose(window))
    {
        /* calculate delta time */
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        glfwPollEvents();

        if (!changedDirection) {
            
            /* manage user input */
            changedDirection = Snake.ProcessInput(deltaTime);

        }
        
        if (deltaTime >= 1 / FRAMERATE) {

            /* update game state */
            Snake.Update(deltaTime);

            /* render */
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Snake.Render();

            glfwSwapBuffers(window);

            lastFrame = currentFrame;
            changedDirection = false;
        }
    
    }

    /* delete all resources as loaded using the resource manager */
    Utility::ResourceManager::Clear();

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    const unsigned int LOWER_LEFT_CORNER_X_COORDINATE = 0;
    const unsigned int LOWER_LEFT_CORNER_Y_COORDINATE = 0;
    glViewport(LOWER_LEFT_CORNER_X_COORDINATE, LOWER_LEFT_CORNER_Y_COORDINATE, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    /* when a user presses the escape key, we set the WindowShouldClose property to true, closing the application */
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, true);
    
    if (0 <= key && 1024 > key)
    {
        if (action == GLFW_PRESS)
            Snake.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Snake.Keys[key] = false;

    }
}

void initializeGLFW()
{
    /* Initializes GLFW library */
    if (glfwInit() == GL_FALSE)
    {
        std::runtime_error ex("Error in initializing GLFW library.");
        throw ex;
    }

    const unsigned int OPENGL_VERSION_MAJOR = 3;
    const unsigned int OPENGL_VERSION_MINOR = 3;

    /* glfwWindowHint sets hint for the next call to glfwCreateWindow */
    /* Set OpenGL version as 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);

    /* Use GLFW core profile */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, false);
}

GLFWwindow* initializeWindow()
{
    try
    {
        initializeGLFW();
    }
    catch (std::runtime_error& ex) {
        throw ex;
    }

    const std::string WINDOW_NAME = "Snake";

    /* Create Window */
    GLFWwindow* window = glfwCreateWindow(game::SCREEN_WIDTH, game::SCREEN_HEIGHT, WINDOW_NAME.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        std::runtime_error ex("Failed to create GLFW window");
        throw ex;
    }
    glfwMakeContextCurrent(window);

    /* Initialize GLAD */
    /* GLAD manges function pointers for OpenGL */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::runtime_error ex("Failed to initialize GLAD");
        throw ex;
    }

    const unsigned int LOWER_LEFT_CORNER_X_COORDINATE = 0;
    const unsigned int LOWER_LEFT_CORNER_Y_COORDINATE = 0;

    /* Viewport */
    /* Set size of rendering window. */
    glViewport(LOWER_LEFT_CORNER_X_COORDINATE, LOWER_LEFT_CORNER_Y_COORDINATE, game::SCREEN_WIDTH, game::SCREEN_HEIGHT);

    glfwSetKeyCallback(window, key_callback);

    /* To tell GLFW to call framebuffer_size_callback to resize the rendering window on every window resize. */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;

}
