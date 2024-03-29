#include "pch.h"
#include <glad/glad.h>
#include <Eigen/Dense>

#include <glm/glm.hpp>
#include <iostream>

#include "Assets/Camera/Camera.h"
#include "Assets/Geometry/TriangleSurface.h"
#include "Assets/Shader/Shader.h"
#include "Assets/Structure/XYZ.h"

// imgui
#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"


#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
// #include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <unordered_map>
#include <GLFW/glfw3.h>

#include "Assets/House.h"
#include "Assets/KeySwitch.h"
#include "Assets/Axis/InteractiveObject.h"
#include "Assets/Camera/FirstPersonController.h"
#include "Assets/Camera/FlyCameraController.h"
#include "Assets/Camera/ThirdPersonController.h"
#include "Assets/Structure/BSpline.h"
#include "Assets/Structure/CameraMatricies.h"
#include "Assets/Structure/playerObject.h"
#include "Assets/Structure/Enemy.h"
#include "Assets/Structure/GeneralVisualObject.h"
#include "Assets/Structure/ModelVisualObject.h"
#include "Assets/Structure/Trophy.h"
#include "Vendor/imgui/imgui_internal.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
typedef std::pair<std::string, KT::VisualObject*> MapPair;

// Camera camera = (glm::vec3(0.f, 2.f, -5.f));

// std::shared_ptr<Camera> camera1 = std::make_shared<Camera>(glm::vec3(0.f, 2.f, -5.f));
// std::shared_ptr<Camera> camera1;
// std::shared_ptr<Camera> camera2 = std::make_shared<Camera>(glm::vec3(0.f, 2.f, -5.f));
std::shared_ptr<Camera> activeCamera = std::make_unique<Camera>(glm::vec3(0.f, 2.f, 3.f));
std::shared_ptr<IController> camera_controller;
std::shared_ptr<IController> thirdPersonController;
std::shared_ptr<IController> firstPersonController;

float mouseLastX = SCR_WIDTH / 2.f;
float mouseLastY = SCR_HEIGHT / 2.f;
bool bFirstMouse = true;

//very ugly time global variables
float deltaTime = 0.1f; // the time between the current and last frame
float lastFrame = 0.0f; // the of last frame

// possesed
KT::InteractiveObject* currentPossesedObject = nullptr;
bool bDrawNormals = false;
float drawNormalLength = 0.3f;


// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
// decalring functions

// setup trophies
void setupTrophies(std::unordered_map<std::string, KT::VisualObject*>& mMap, KT::playerObject* player,
                   const std::unordered_map<std::string, KTTexture2D>& textures,
                   unsigned int& currentPlayerScore, Shader* unlitTextureShader, KT::TriangleSurface* surface1, bool initialize = false) {
    currentPlayerScore = 0;
    // Exam task 5
    for (int i = 0; i < 6; ++i) {
        float x = KT::Random::Random(0, 1.f);

        float z = KT::Random::Random(0, 1.f);
        KT::Trophy* trophy = new KT::Trophy(player, 0.01f, currentPlayerScore);
        trophy->UseShader(unlitTextureShader);
        trophy->AddTexture(textures.at("party").textureID);
        trophy->SetPosition(surface1->FindPointOnSurfaceXZ(glm::vec3(x, 0, z)));
        std::string trophyName = "t" + std::to_string(i);
        if (mMap.count(trophyName)) {
            delete mMap.at(trophyName);
            mMap.erase(trophyName);
        }
        
        if (initialize) 
            trophy->init(0);
        
        mMap.insert(MapPair(trophyName, trophy));
    }
}

// SETUP AND CLEANUP
void setupVisualObjects(std::unordered_map<std::string, KT::VisualObject*>& mMap,
                        const std::unordered_map<std::string, KTTexture2D>& textures,
                        unsigned int& currentPlayerScore, Shader* textureShader, Shader* unlitTextureShader) {
    // temporary containers, used to insert custom data into Visual Objects
    std::vector<KT::Vertex> vertices{};
    std::vector<int> indices{};

    // Exam task 3
    // second surface
    KT::TriangleSurface* surface1 = new KT::TriangleSurface(textureShader, textures.at("heightmap").textureID);
    surface1->constructWithTexture(textures.at("heightmap"));
    surface1->SetPosition(0, 0, 0);
    surface1->SetupTriData();
    // Exam task 10
    surface1->CalculateNormals();
    mMap.insert(MapPair("surface", surface1));

    /*
    KT::FileHandler::FromAssimp("Assets/Art/Models/cube.fbx", vertices, indices);
    KT::GeneralVisualObject* modelVis = new KT::GeneralVisualObject(vertices, indices);
    modelVis->UseShader(textureShader);
    modelVis->AddTexture(textures.at("rick").textureID);
    modelVis->SetPosition(0, 0.1f, 0);
    modelVis->SetScale(0.1f);
    modelVis->SetRotation(glm::vec3(180.f, 0, 0));
    mMap.insert(MapPair("model_vis", modelVis));
    */


    // spline
    /*
    std::vector<glm::vec3> splinePoints{};
    splinePoints.push_back(glm::vec3(0, 0, 0));
    splinePoints.push_back(glm::vec3(1, 0, 0));
    splinePoints.push_back(glm::vec3(2, 1, 0));
    splinePoints.push_back(glm::vec3(0, 2, 0));
    splinePoints.push_back(glm::vec3(0, 4, 2));
    splinePoints.push_back(glm::vec3(0, 4, 4));
    KT::VisualObject* bSpline = new KT::BSpline(splinePoints, 3);
    mMap.insert(MapPair("b_spline", bSpline));
    */

    KT::VisualObject* xyz = new KT::XYZ();
    xyz->name = "XYZ";
    xyz->SetPosition(glm::vec3(0, 0, 0));
    mMap.insert(std::pair<std::string, KT::VisualObject*>{"xyz", xyz});

    vertices.clear();
    indices.clear();
    // Exam task 11   
    KT::FileHandler::Import_obj_importer("Assets/Art/Models/objcube.obj", vertices, indices);
    KT::playerObject* player = new KT::playerObject(surface1, vertices, indices, textureShader); // Exam task 4
    // Exam task 2
    player->AddTexture(textures.at("wall").textureID);
    player->SetScale(glm::vec3(0.010f));
    player->name = "CUBE";
    mMap.insert(std::pair<std::string, KT::VisualObject*>{"player", player});
    // std::shared_ptr<KT::InteractiveObject> aa(cube);

    thirdPersonController = std::make_unique<KT::ThirdPersonController>(activeCamera, player);
    firstPersonController = std::make_unique<KT::FirstPersonController>(activeCamera, player);

    camera_controller = thirdPersonController;
    // camera_controller = std::make_unique<KT::FlyCameraController>(activeCamera);


    // Exam task 11   
    KT::KeySwitch* key = new KT::KeySwitch(player, "Assets/Art/Models/key.obj");
    key->UseShader(textureShader);
    key->AddTexture(textures.at("wall").textureID);
    key->SetPosition(0.35f, 0.05, 0.1);
    mMap.insert(MapPair("key", key));

    // TROPHIES
    // ----------------------------------------
    setupTrophies(mMap, player, textures, currentPlayerScore, unlitTextureShader, surface1);

    // ENEMIES
    // -----------------------------------------------------------------------------------------------------------------
    for (int i = 0; i < 4; ++i) {
        float x = KT::Random::Random(0, 1.f);

        float z = KT::Random::Random(0, 1.f);

        // std::cout << "xz : " << x << " " << z <<std::endl;
        KT::Enemy* enemy = new KT::Enemy(player, 0.01f);
        enemy->SetPosition(surface1->FindPointOnSurfaceXZ(glm::vec3(x, 0, z)));
        mMap.insert(MapPair("enemy_" + std::to_string(i), enemy));
    }

    std::vector<KT::Vertex> lightMeshVerts = KT::OctahedronBall::makeUnitBall(2);
    for (int i = 0; i < lightMeshVerts.size(); ++i)
        lightMeshVerts[i].set_normal(glm::vec3(1.f));

    KT::VisualObject* lightMesh = new KT::GeneralVisualObject(lightMeshVerts);
    lightMesh->SetPosition(0.5, 0.1f, 0.5);
    lightMesh->SetScale(glm::vec3(0.01f));
    mMap.insert(MapPair("light_mesh", lightMesh));

    // counter
    KT::GeneralVisualObject* counter = new KT::GeneralVisualObject(KT::GeometryHelpers::planeVertices(),
                                                                   KT::GeometryHelpers::planeIndices());
    counter->UseShader(textureShader);
    counter->AddTexture(textures.at("123").textureID);
    counter->SetPosition(-0.02f, 0.05f, 0.3f);
    counter->SetRotation(glm::vec3(-90,0,0));
    counter->SetScale(0.05f);
    mMap.insert(MapPair("counter", counter));

    KT::FileHandler::FromAssimp("Assets/Art/Models/Door.fbx", vertices, indices);
    KT::GeneralVisualObject* sampleObject1 = new KT::GeneralVisualObject(vertices, indices);
    sampleObject1->SetScale(0.04f);
    sampleObject1->SetPosition(0.5f, 0.04f, 0.5f);
    sampleObject1->UseShader(textureShader);
    sampleObject1->AddTexture(textures.at("heightmap").textureID);
    mMap.insert(MapPair("sample1", sampleObject1));

    
    KT::FileHandler::FromAssimp("Assets/Art/Models/cubee.fbx", vertices, indices);
    KT::GeneralVisualObject* sampleObject2 = new KT::GeneralVisualObject(vertices, indices);
    sampleObject2->SetScale(0.04f);
    sampleObject2->SetPosition(1.f, 0.04f, 1.f);
    sampleObject2->SetRotation(90,0,180);
    sampleObject2->UseShader(textureShader);
    sampleObject2->AddTexture(textures.at("wall").textureID);
    mMap.insert(MapPair("sample2", sampleObject2));
}

void cleanupVisualObjects(std::unordered_map<std::string, KT::VisualObject*>& mMap) {
    for (auto visualElement : mMap) {
        delete visualElement.second;
    }

    mMap.clear();
}


// global easy

// CALLBACKS
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// GENEREAL SETUP

/**
 * \brief This sets up the backend of the program, need only be called once
 * \param outWindow 
 * \return a
 */
int setupGLFW_IMGUI_glad(GLFWwindow*& outWindow) {
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
    outWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (outWindow == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(outWindow);
    glfwSetFramebufferSizeCallback(outWindow, framebuffer_size_callback);
    glfwSwapInterval(1);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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
        ImGui_ImplGlfw_InitForOpenGL(outWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    // CALLBACKS
    // ----------------------------------------

    //mouse settings
    //------------------------------------------------
    //hide and capture mouse
    glfwSetInputMode(outWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //setup mouse input callback
    // glfwSetCursorPosCallback(window, mouseCallback); //TODO NOTE THIS DISABLES IMGUI

    //scroll callback
    glfwSetScrollCallback(outWindow, scrollCallback);

    return 0;
}

void cleanupGLFW_IMGUI_glad(GLFWwindow* window) {
    // IMGUI CLEANUP
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
}


/*// falloff func
float falloffFunc(float x) {
    return cos(4.f * x) * 1.f / exp(x);
}*/


int main() {
    GLFWwindow* window{};
    int valid = setupGLFW_IMGUI_glad(window);
    if (valid != 0)
        return valid;


    KT::geometry_helpers::getInstance().init(0);

    // USER STUFF
    // ----------------------------------------

    unsigned int currentPlayerScore = 0;

    // objects in scene
    std::unordered_map<std::string, KT::VisualObject*> mMap{};
    std::unordered_map<std::string, KTTexture2D> mTextures{};

    // Shaders
    Shader leksjon2Shader = Shader("Assets/Art/Shaders/Lek2V.glsl",
                                   "Assets/Art/Shaders/Lek2F.glsl");
    leksjon2Shader.use();
    GLint matrixUniform = glGetUniformLocation(leksjon2Shader.ID, "matrix");

    Shader* textureShader = new Shader("Assets/Art/Shaders/SimpleTexV.glsl",
                                       "Assets/Art/Shaders/SimpleTexF.glsl");
    textureShader->use();
    Shader* unlitTextureShader = new Shader("Assets/Art/Shaders/UnlitTextureV.glsl",
                                            "Assets/Art/Shaders/UnlitTextureF.glsl");
    unlitTextureShader->use();

    // Textures

    mTextures.insert(std::pair<std::string, KTTexture2D>("123", KTTextureFromFile("Assets/Textures/123.png")));
    mTextures.insert(std::pair<std::string, KTTexture2D>("rick", KTTextureFromFile("Assets/Textures/rick.jpg")));
    mTextures.insert(std::pair<std::string, KTTexture2D>("heightmap", KTTextureFromFile("Assets/Textures/render.png")));
    mTextures.insert(std::pair<std::string, KTTexture2D>("party", KTTextureFromFile("Assets/Textures/party.jpg")));
    mTextures.insert(std::pair<std::string, KTTexture2D>("grass", KTTextureFromFile("Assets/Textures/metal.jpg")));
    mTextures.insert(std::pair<std::string, KTTexture2D>("wall", KTTextureFromFile("Assets/Textures/wall.jpg")));

    setupVisualObjects(mMap, mTextures, currentPlayerScore, textureShader, unlitTextureShader);

    // Initialize Visual Objects
    for (auto object : mMap) {
        object.second->init(matrixUniform);
    }

    // Good pratice to unbind vertex arrays
    glBindVertexArray(0);

    // binding geometry shader
    Shader normalGeoShader = Shader("Assets/Art/Shaders/NormalGeoV.glsl", "Assets/Art/Shaders/NormalGeoF.glsl",
                                    "Assets/Art/Shaders/NormalGeoG.glsl");

    glm::mat4x4 model = glm::mat4x4(1.f);
    glm::mat4x4 projection = glm::perspective(glm::radians(activeCamera->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                              0.001f,
                                              100.f);

    // LEKSJON 2
    // ----------------------------------------
    // OTHER ENABLES
    // -----------------------------------------------------------------------------------------------------------------
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //setting up depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    // RENDER LOOP
    // -----------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
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
                "WASD KEYS    - Move Player\n"
                "MOVE MOUSE   - Look around\n"
                "C            - Toggle Camera\n");
            ImGui::End();

            ImGui::Begin("Hello World!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Checkbox("Draw Normals", &bDrawNormals);
            if (bDrawNormals) {
                ImGui::SliderFloat("NormalVectorLength", &drawNormalLength, 0.01f, 2.f);
            }

            // ImGui::Checkbox("GraphToggle", &graphNPCWalker->toggle);
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

        glm::mat4x4 view = activeCamera->GetViewMatrix();
        CameraPosition = activeCamera->position;
        projection = glm::perspective(glm::radians(activeCamera->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f,
                                      100.f);
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

        // Exam task 6
        if (((KT::playerObject*)(mMap.at("player")))->mDead) {
            std::cout << "Player is dead, resetting game" << std::endl;
            auto player = ((KT::playerObject*)(mMap.at("player")));
            player->mDead = false;
            player->ResetToTriangle(1);
            currentPlayerScore = 0;
            setupTrophies(mMap, player, mTextures, currentPlayerScore, unlitTextureShader,
                ((KT::TriangleSurface*)(mMap.at("surface"))), true);
            
        }

        // mMap["disc"]->SetPosition(mMap["disc"]->GetPosition() += glm::vec3(1,0,0)*deltaTime*2.f);
        for (auto object : mMap) {
            object.second->Update(deltaTime);
        }

        CameraProjection = projection;
        CameraView = view;

        leksjon2Shader.use();
        leksjon2Shader.setMat4("projection", CameraProjection);
        leksjon2Shader.setMat4("view", CameraView);


        leksjon2Shader.setMat4("matrix", glm::mat4(1.f));

        // Exam task 8
        // setting shader light uniforms
        glm::vec3 lightPos = glm::vec3(0.5f * sin(glfwGetTime() + 0.5f), 0.3f, 0.5f * cos(glfwGetTime() + 0.5f));
        lightPos += glm::vec3(0.5f, 0.f, 0.5f);
        mMap.at("light_mesh")->SetPosition(lightPos);

        textureShader->use();
        textureShader->setVec3("objectColor", 1.0f, 1.f, 1.f);

        // Task 9
        if (((KT::KeySwitch*)(mMap.at("key")))->IsOn()) {
            textureShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        }
        else {
            textureShader->setVec3("lightColor", 0.1f, 0.1f, 0.1f);
        }

        textureShader->setVec3("lightPos", lightPos);
        textureShader->setVec3("viewPos", CameraPosition);


        for (auto object : mMap) {
            leksjon2Shader.use();
            object.second->draw();
        }

        if (bDrawNormals) {
            normalGeoShader.use();
            normalGeoShader.setFloat("MAGNITUDE", drawNormalLength);
            normalGeoShader.setMat4("model", model);
            normalGeoShader.setMat4("view", view);
            normalGeoShader.setMat4("projection", projection);
            for (auto m_object : mMap) {
                m_object.second->forceDrawElements();
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

    cleanupGLFW_IMGUI_glad(window);

    // other cleanup
    // delete textureShader;
    // delete[] texture_2d.data;

    cleanupVisualObjects(mMap);

    for (auto texture : mTextures) {
        delete texture.second.data;
    }
    delete textureShader;
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static bool UI_enabled;
static bool bbbb = false;
bool cameraSwitchButton = false;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!bbbb) {
            bbbb = true;
            UI_enabled = !UI_enabled;
            if (UI_enabled) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
    else {
        bbbb = false;
    }

    //movement
    glm::vec3 keyboardAxis = glm::vec3(0.f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        keyboardAxis.z += 1.f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        keyboardAxis.z += -1.f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        keyboardAxis.x += 1.f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        keyboardAxis.x += -1.f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        keyboardAxis.y += -1.f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        keyboardAxis.y += 1.f;

    // Exam task 1
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!cameraSwitchButton) {
            if (camera_controller == firstPersonController)
                camera_controller = thirdPersonController;
            else
                camera_controller = firstPersonController;
            cameraSwitchButton = true;
        }
    }
    else {
        cameraSwitchButton = false;
    }
    camera_controller->ProcessKeyboard(keyboardAxis, deltaTime);

    // possesed object
    float moveScalar = 0.01f;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        // currentPossesedObject->move(0, -moveScalar, KT::Graph::Franke);
        currentPossesedObject->move(0, 0, -moveScalar);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        // currentPossesedObject->move(0, moveScalar, KT::Graph::Franke);
        currentPossesedObject->move(0, 0, +moveScalar);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        // currentPossesedObject->move(-moveScalar, 0, KT::Graph::Franke);
        currentPossesedObject->move(-moveScalar, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        currentPossesedObject->move(moveScalar, 0, 0);
        // currentPossesedObject->move(moveScalar, 0, KT::Graph::Franke);
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (bFirstMouse) {
        bFirstMouse = false;
        mouseLastX = xpos;
        mouseLastY = ypos;
    }
    float offsetX = xpos - mouseLastX;
    float offsetY = ypos - mouseLastY;
    mouseLastX = xpos;
    mouseLastY = ypos;

    if (!UI_enabled) {
        camera_controller->ProcessMouseMovement(offsetX, offsetY, true, true);
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera_controller->ProcessMouseScroll(yoffset, true);
    }
    else {
        camera_controller->ProcessMouseScroll(yoffset, false);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
