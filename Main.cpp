#include "shader_utils.h"
#include "shapes.h"
#include "window_setup.h"

// Window Dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;
GLFWwindow* mainWindow;

// Shaders, and their inputting
std::string vShader = ReadShaderFile("vertex_shader.vert");
std::string fShader = ReadShaderFile("fragment_shader.frag");

int main() {

    // Error?
    if (!glfwInit()) {
        printf("GLFW Initialization failed!");
        glfwTerminate();
        return 1;
    };

    // Creates the window, sets it to the OpenGL context
    mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "TEST WINDOW", NULL, NULL);
    glfwMakeContextCurrent(mainWindow);
    
    // Error?
    if (!mainWindow) {
        printf("GLFW Window Creation Failed!");
        glfwTerminate();
        return 1;
    }

    // Sets and gets the viewport buffer sizes
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    
    // Error?
    if (glewInit() != GLEW_OK) {
        printf("GLEW Initalization Failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Depth test to avoid over-rendering
    glEnable(GL_DEPTH_TEST);

    // Just sets the actual viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Creates the actual pyramid and buffers it
    CreatePyramid(VAO, VBO, IBO);

    // Compiles shaders, adds them to the pipeline
    CompileShaders(&shader, &uniformModel, &uniformProjection, vShader, fShader);

    // Angle declaration used for rotation transformation
    float angle = 0;

    while (!glfwWindowShouldClose(mainWindow)) {

        // Used to handle user input and events
        glfwPollEvents();
       
        // Clears the screen to a certain color
        glClearColor(1.0f, 0.5f, 0.0f, 0.0f);

        // Clears pixel buffers, colors and depths
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Links compiled shader to our program
        glUseProgram(shader);

        // Sets up the affine transformation matrices to be used on our object
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3 * glfwGetTime()));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));

        // Handles angles for rotation
        angle = (angle <= 360) ? angle + 0.005f : 0;

        // Initializing the projection matrix
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.0001f, 100.0f);
       
        // Setting up and linking the Projection, View and Model matrices
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Binding buffers and arrays
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        // Draws the actual objects
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // Unbinding everything
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);

        // Swaps back and front buffers
        glfwSwapBuffers(mainWindow);
    }

    // Bye-bye
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
