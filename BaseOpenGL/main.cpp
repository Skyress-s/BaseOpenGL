#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <iostream>

#include "Assets/Camera/Camera.h"
#include "Assets/Geometry/TriangleSurface.h"
#include "Assets/Model/Model.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Structure/Tetrahedron.h"
#include "Assets/Structure/XYZ.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera = (glm::vec3(0.f, 0.f, 3.f));
float mouseLastX = SCR_WIDTH / 2.f;
float mouseLastY = SCR_HEIGHT / 2.f;
bool bFirstMouse = true;

//very ugly time global variables
float deltaTime = 0.1f; // the time between the current and last frame
float lastFrame = 0.0f; // the of last frame


// TODO delete shaders
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// decalring functions

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // callbacks

    //mouse settings
    //------------------------------------------------
    //hide and capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //setup mouse input callback

    glfwSetCursorPosCallback(window, mouseCallback);

    //scroll callback
    glfwSetScrollCallback(window, scrollCallback);


    // USER STUFF
    // ----------------------------------------

    // objects in scene
    std::vector<VisualObject*> mObjects{};
    mObjects.push_back(new MM::XYZ());
    mObjects.push_back(new MM::Tetrahedron());
    mObjects.push_back(new MM::TriangleSurface());
    static_cast<MM::TriangleSurface*>(mObjects[mObjects.size()-1])->construct();

    // Getting shader
    Shader leksjon2Shader = Shader("Assets/Art/Shaders/Lek2V.glsl", 
"Assets/Art/Shaders/Lek2F.glsl");
    leksjon2Shader.use();

    GLint matrixUniform = glGetUniformLocation(leksjon2Shader.ID, "matrix");

    // Initializing Visual Objects
    for (VisualObject* m_object : mObjects) {
        m_object->init(matrixUniform);
    }
    
    // Good pratice to unbind vertex arrays
    glBindVertexArray(0);
    


    Shader mainShader = Shader("Assets/Art/Shaders/SSimpleEmissionV.glsl", "Assets/Art/Shaders/SSimpleEmissionF.glsl");

    glm::mat4x4 model = glm::mat4x4(1.f);
    glm::mat4x4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,
                                              100.f);

    // LEKSJON 2
    // ----------------------------------------
    // OTHER ENABLES
    // -----------------------------------------------------------------------------------------------------------------
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //setting up depth test
    glEnable(GL_DEPTH_TEST);

    
    // RENDER LOOP
    // -----------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // TIME
        // -----------------------------------------------------------------------------------------------------------------
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;
        
        // input
        // -----
        processInput(window);

        glm::mat4x4 view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
        
        // RENDER
        // -----------------------------------------------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // THIS IS THE SECTION WHERE WE RENDER AND DO OUR LOGIC
        // -----------------------------------------------------------------------------------------------------------------
        
        leksjon2Shader.use();
        leksjon2Shader.setMat4("projection", projection);
        leksjon2Shader.setMat4("view", view);

        for (VisualObject* object : mObjects) {
            object->draw();
        }
        // for (std::vector<VisualObject*>::iterator it = mObjects.begin(); it != mObjects.end(); it++) {
        //     (*it)->draw();
        // }
        

        
        // CLEAN UP / END OF FRAME RELEATED
        // -----------------------------------------------------------------------------------------------------------------
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


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

    //movement
    glm::vec3 keyboardAxis = glm::vec3(0.f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        keyboardAxis.z += 1.f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        keyboardAxis.z += -1.f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        keyboardAxis.x += -1.f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        keyboardAxis.x += 1.f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        keyboardAxis.y += -1.f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        keyboardAxis.y += 1.f;
    camera.ProcessKeyboard(keyboardAxis, deltaTime);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (bFirstMouse)
    {
        bFirstMouse = false;
        mouseLastX = xpos;
        mouseLastY = ypos;
    }
    float offsetX = xpos - mouseLastX;
    float offsetY = ypos - mouseLastY;
    mouseLastX = xpos;
    mouseLastY = ypos;

    camera.ProcessMouseMovement(offsetX, offsetY, true, true);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ProcessMouseScroll(yoffset, true);
    }
    else
    {
        camera.ProcessMouseScroll(yoffset, false);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
