#include <glad/glad.h>


#include <glm/glm.hpp>
#include <iostream>

#include "Assets/Camera/Camera.h"
#include "Assets/Geometry/TriangleSurface.h"
#include "Assets/Model/Model.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Structure/Tetrahedron.h"
#include "Assets/Structure/XYZ.h"

// imgui
#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"
#include <stdio.h>


#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
// #include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <GLFW/glfw3.h>

#include "Assets/Axis/InteractiveObject.h"
#include "Assets/Structure/Cube.h"
#include "Assets/Structure/Disc.h"
#include "Assets/Structure/Graph2D.h"
#include "Assets/Structure/OctahedronBall.h"
#include "Assets/VisualObjectUI/TransformUI.h"
#include "Vendor/imgui/imgui_internal.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

Camera camera = (glm::vec3(0.f, 0.f, 5.f));
float mouseLastX = SCR_WIDTH / 2.f;
float mouseLastY = SCR_HEIGHT / 2.f;
bool bFirstMouse = true;

//very ugly time global variables
float deltaTime = 0.1f; // the time between the current and last frame
float lastFrame = 0.0f; // the of last frame

// possesed
MM::InteractiveObject* currentPossesedObject = nullptr;
bool bDrawNormals = false;
float drawNormalLength = 0.3f;

// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
// decalring functions

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


// falloff func
float falloffFunc(float x)
{
    return cos(4.f * x) * 1.f / exp(x);
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
GLM_FUNC_QUALIFIER void print(glm::mat<C, R, T, Q> const& m)
{
    for (int y = 0; y < R; ++y)
    {
        for (int x = 0; x < C; ++x)
        {
            std::cout << m[x][y];
        }
        std::cout << std::endl;
    }
}


template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
void SolveThreePlanes(glm::mat<C, R, T, Q> mat, glm::vec<R, T, Q> equals)
{
    static_assert(R == C, "Rows and Collums has to be equal!");
    // example how to do it
    // glm::mat3 testMat = glm::mat3(1.f);
    // testMat[0][0] = 1.f;
    // testMat[1][0] = 1.f;
    // testMat[2][0] = 1.f;
    //
    // testMat[0][1] = 5.f;
    // testMat[1][1] = 3.f;
    // testMat[2][1] = 2.f;
    //
    // testMat[0][2] = 1.f;
    // testMat[1][2] = 3.f;
    // testMat[2][2] = 2.f;
    std::cout << "MATRIX" << std::endl;
    print(mat);

    glm::mat<C, R, T, Q> inv = inverse(mat);


    glm::vec<R, T, Q> res = inv * equals;
    std::cout << "RESULTS XYZW VALUES" << std::endl;
    for (int i = 0; i < R; ++i)
    {
        std::cout << res[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "RECALCULATE" << std::endl;
    for (int y = 0; y < C; ++y)
    {
        float plane = 0.f;
        for (int x = 0; x < C; ++x)
        {
            plane += mat[x][y] * res[x];
        }
        std::cout << plane << " ";
    }
    std::cout << std::endl;

    std::cout << "SHOULD EQUAL" << std::endl;
    for (int i = 0; i < R; ++i)
    {
        std::cout << equals[i] << " ";
    }
    std::cout << std::endl;


    return;
}

int main()
{
    if (true)
    {
        glm::mat3 testMat = glm::mat3(1.f);
        float arr[] = {
            1, 5, 1,
            1, 3, 3,
            1, 2, 2
        };
        testMat = glm::make_mat3(arr);
        print(testMat);
        // testMat[0][0] = 1.f;
        // testMat[1][0] = 1.f;
        // testMat[2][0] = 1.f;
        //
        // testMat[0][1] = 5.f;
        // testMat[1][1] = 3.f;
        // testMat[2][1] = 2.f;
        //
        // testMat[0][2] = 1.f;
        // testMat[1][2] = 3.f;
        // testMat[2][2] = 2.f;
        SolveThreePlanes(testMat, glm::vec3(1, 2, 1));
    }
    else
    {
        glm::mat4 w2t2_mat = glm::mat4(0.f);

        w2t2_mat[0][0] = 1.f / 16.f;
        w2t2_mat[1][0] = -0.5f;
        w2t2_mat[2][0] = 3.f / 2.f;
        w2t2_mat[3][0] = -1.f;

        w2t2_mat[0][1] = 0.5f;
        w2t2_mat[1][1] = -2.f;
        w2t2_mat[2][1] = 3.f / 2.f;
        w2t2_mat[3][1] = -1.f;

        w2t2_mat[0][2] = 4.f;
        w2t2_mat[1][2] = -8.f;
        w2t2_mat[2][2] = 6.f;
        w2t2_mat[3][2] = -1.f;

        w2t2_mat[0][3] = 27.f / 2.f;
        w2t2_mat[1][3] = -18.f;
        w2t2_mat[2][3] = 9.f;
        w2t2_mat[3][3] = -1.f;

        SolveThreePlanes(w2t2_mat, glm::vec4(1.f / 16.f, 0.5f, 1.f, 7.f / 2.f));
    }


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

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
    glfwSwapInterval(1);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // IMGUI
    // ----------------------------------------

    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Imgui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    // CALLBACKS
    // ----------------------------------------

    //mouse settings
    //------------------------------------------------
    //hide and capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //setup mouse input callback
    // glfwSetCursorPosCallback(window, mouseCallback); //TODO NOTE THIS DISABLES IMGUI

    //scroll callback
    glfwSetScrollCallback(window, scrollCallback);


    // USER STUFF
    // ----------------------------------------

    // objects in scene
    std::vector<VisualObject*> mObjects{};

    // disc
    MM::Disc* disc = new MM::Disc();
    disc->construct(100);
    disc->SetPosition(glm::vec3(4,3,0));
    mObjects.push_back(disc);
    

    MM::TriangleSurface* plane1 = new MM::TriangleSurface();
    plane1->SetPosition(glm::vec3(0, 0, 0));
    auto plane1Func = [](float x, float z)
    {
        return 1.f - x - z;
    };
    plane1->constructWithLambda(plane1Func);
    mObjects.push_back(plane1);

    MM::TriangleSurface* plane2 = new MM::TriangleSurface();
    plane2->SetPosition(glm::vec3(0, 0, 0));
    auto plane2Func = [](float x, float z)
    {
        return (2.f - 5.f * x - 2.f * z) / 3.f;
    };
    plane2->constructWithLambda(plane2Func);
    mObjects.push_back(plane2);

    MM::TriangleSurface* plane3 = new MM::TriangleSurface();
    plane3->SetPosition(glm::vec3(0, 0, 0));
    auto plane3Func = [](float x, float z)
    {
        return 2.f;
        return (1.f - 1.f * x - 2.f * z) / 3.f;
    };
    plane3->constructWithLambda(plane3Func);
    mObjects.push_back(plane3);


    VisualObject* xyz = new MM::XYZ();
    xyz->name = "XYZ";
    xyz->SetPosition(glm::vec3(0, 0, 0));
    mObjects.push_back(xyz);

    // MM::Tetrahedron* tet = new MM::Tetrahedron();
    // tet->name = "TETRAHEDRON";
    // tet->SetPosition(glm::vec3(0, 0, -4));
    // mObjects.push_back(tet);

    MM::InteractiveObject* cube = new MM::Cube();
    cube->SetPosition(glm::vec3(0.25f, -0.75f, 1.5f));
    cube->SetScale(glm::vec3(0.8f));
    cube->name = "CUBE";
    currentPossesedObject = cube;
    mObjects.push_back(cube);

    MM::TriangleSurface* tri1 = new MM::TriangleSurface();
    tri1->name = "TRIANGLE SURFACE GRAPH";
    tri1->SetPosition(glm::vec3(4, 0, 0));
    tri1->construct(); // makes the functions from task 2
    // tri1->toFile("surface.txt");
    // tri1->readFile("surface.txt");
    mObjects.push_back(tri1);


    auto ff = [](float x)
    {
        return cos(4.f * x) * 1.f / exp(x);
    };

    // 2d falloff func
    MM::Graph2D* graph_2d = new MM::Graph2D(ff, 20, 0.f, 5.f);
    graph_2d->name = "GRAPH 2D";
    // graph_2d->toFile("FalloffGraph.txt");
    graph_2d->readFile("FalloffGraph.txt");
    graph_2d->SetPosition(glm::vec3(-2, 3, 0));
    mObjects.push_back(graph_2d);

    // lissa graph
    auto lissa = [](float t)
    {
        float d = glm::pi<float>() / 2.f;
        float a = 3.f;
        float b = 4.f;
        float x = 1.f * sin(a * t + d);
        float y = 1.f * sin(b * t);

        return MM::Vertex(x, y, 0.f, 1.f, 1.f, 1.f);
    };
    std::vector<MM::Vertex> lissaVerts{};
    for (float i = 0; i < glm::pi<float>() * 2.f; i += 0.1f)
    {
        lissaVerts.push_back(lissa(i));
    }

    MM::Graph2D* lissaGraph = new MM::Graph2D(lissaVerts);
    lissaGraph->SetPosition(glm::vec3(-3, 0, 0));
    lissaGraph->name = "LISSAJOUS CURVE";
    // lissaGraph->toFile("LissaGraph.txt");
    lissaGraph->readFile("LissaGraph.txt");
    mObjects.push_back(lissaGraph);


    auto oblig1_3Func = [](float x, float y)
    {
        // return 1.f;
        return (1 - x - y);
    };
    MM::TriangleSurface* oblig1_3Graph = new MM::TriangleSurface();
    oblig1_3Graph->SetPosition(glm::vec3(0, 0, 0));
    oblig1_3Graph->constructWithLambda(oblig1_3Func);
    mObjects.push_back(oblig1_3Graph);


    // getting the integral
    float lower, upper, step;
    lower = 0.f;
    upper = 1.f;
    step = 0.0025f;

    float total{};
    // reads from the center of each square we evaluate 
    for (float x = lower + 0.5f * step; x < upper; x += step)
    {
        for (float y = lower + 0.5f * step; y < 1.f - x; y += step)
        {
            total += oblig1_3Func(x, y) * step * step; // height * length * length       
        }
    }

    std::cout << total << std::endl;

    MM::OctahedronBall* octBall = new MM::OctahedronBall(3);
    octBall->SetPosition(glm::vec3(0, 2, 3));
    mObjects.push_back(octBall);


    // Getting shader
    Shader leksjon2Shader = Shader("Assets/Art/Shaders/Lek2V.glsl",
                                   "Assets/Art/Shaders/Lek2F.glsl");
    leksjon2Shader.use();
    GLint matrixUniform = glGetUniformLocation(leksjon2Shader.ID, "matrix");

    // Initializing Visual Objects
    for (VisualObject* m_object : mObjects)
    {
        m_object->init(matrixUniform);
    }

    // Good pratice to unbind vertex arrays
    glBindVertexArray(0);

    // binding geometry shader
    Shader normalGeoShader = Shader("Assets/Art/Shaders/NormalGeoV.glsl", "Assets/Art/Shaders/NormalGeoF.glsl",
                                    "Assets/Art/Shaders/NormalGeoG.glsl");

    glm::mat4x4 model = glm::mat4x4(1.f);
    glm::mat4x4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,
                                              100.f);


    // UI
    // TransformUI tetUI;
    // tetUI.target = tet;
    TransformUI xyzUI;
    xyzUI.target = xyz;
    // TransformUI cubeUI;
    // cubeUI.target = cube;
    TransformUI triUI;
    triUI.target = tri1;
    TransformUI graphUI;
    graphUI.target = graph_2d;
    TransformUI lissaUI;
    lissaUI.target = lissaGraph;
    // LEKSJON 2
    // ----------------------------------------
    // OTHER ENABLES
    // -----------------------------------------------------------------------------------------------------------------
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //setting up depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    // RENDER LOOP
    // -----------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        double* x = new double();
        double* y = new double();
        glfwGetCursorPos(window, x, y);
        mouseCallback(window, *x, *y);

        // TIME
        // -----------------------------------------------------------------------------------------------------------------
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        // IMGUI
        // -----------------------------------------------------------------------------------------------------------------
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // ImGui::ShowDemoWindow();
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("Controls");

            ImGui::Text("TAB - Enter/exit UI mode\n"
                "ARROW KEYS   - Move cube\n"
                "WASD KEYS    - Move Camera\n"
                "MOVE MOUSE   - Look around");
            ImGui::End();

            ImGui::Begin("Helalo, world!"); // Create a window called "Hello, world!" and append into it.
            // tetUI.Draw();
            xyzUI.Draw();
            // cubeUI.Draw();
            triUI.Draw();
            graphUI.Draw();
            lissaUI.Draw();

            ImGui::Checkbox("Draw Normals", &bDrawNormals);
            if (bDrawNormals)
            {
                ImGui::SliderFloat("NormalVectorLength", &drawNormalLength, 0.01f, 2.f);
            }
            ImGui::End();

            /*
            ImGui::Begin("awdawdHelalo, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))
                // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        */
        }

        glm::mat4x4 view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);

        // INPUT
        // ----------------------------------------
        processInput(window);
        // RENDER
        // -----------------------------------------------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // THIS IS THE SECTION WHERE WE RENDER AND DO OUR LOGIC
        // -----------------------------------------------------------------------------------------------------------------

        // UPDATE
        for (auto object : mObjects)
        {
            object->Update(deltaTime);
        }
        
        leksjon2Shader.use();
        leksjon2Shader.setMat4("projection", projection);
        leksjon2Shader.setMat4("view", view);

        for (VisualObject* object : mObjects)
        {
            object->draw();
        }
        /*
        for (std::vector<VisualObject*>::iterator it = mObjects.begin(); it != mObjects.end(); it++) {
            (*it)->draw();
        }
        */

        if (bDrawNormals)
        {
            normalGeoShader.use();
            normalGeoShader.setFloat("MAGNITUDE", drawNormalLength);
            normalGeoShader.setMat4("model", model);
            normalGeoShader.setMat4("view", view);
            normalGeoShader.setMat4("projection", projection);
            for (auto m_object : mObjects)
            {
                m_object->draw();
            }
        }


        // IMGUI RENDER
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // CLEAN UP / END OF FRAME RELEATED
        // -----------------------------------------------------------------------------------------------------------------
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // IMGUI CLEANUP
    // ----------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static bool UI_enabled;
static bool bbbb = false;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        if (!bbbb)
        {
            bbbb = true;
            UI_enabled = !UI_enabled;
            if (UI_enabled)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
    else
    {
        bbbb = false;
    }

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

    // possesed object
    float moveScalar = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        currentPossesedObject->move(0, 0, -moveScalar);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        currentPossesedObject->move(0, 0, moveScalar);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        currentPossesedObject->move(-moveScalar, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        currentPossesedObject->move(moveScalar, 0, 0);
    }
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

    if (!UI_enabled)
    {
        camera.ProcessMouseMovement(offsetX, offsetY, true, true);
    }
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
