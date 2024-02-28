#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include <vector>
#include<Windows.h>
#include<mmsystem.h>

#pragma commet(lib, "winmm.lib")

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


//unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//variable luz estrella
float attenuationSpeed = 0.5f; //velocidad 
float timeEstrella = 0.0f; // Variable para rastrear el tiempo

//variación de posicion Enemigo roca
float offset = 1.0f; // Posicion inicial 
float offset1 = -5.0f; // Posicion inicial
float direction = 1.0f;
// Velocidad de Rotacion para el Pozo 
float VelocidadRota = 4.0f;

//Goomba
// Variable para controlar la posición actual en el eje Z
float zPos = 2.99797f;
float zOffset = 0.0;

std::vector<Model> backgroundModels;
int currentBackgroundIndex = 0;  // Índice del fondo actual

#pragma comment(lib, "winmm.lib")

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto_Examen_Gr7", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
   // ------------------------------------

    Shader lightingShader("shaders/EB2_vertexshader_model.vs", "shaders/EB2_fragmentshader_model.fs");//shader para la iluminacion y modelos
    Shader lightModelShader("shaders/EB2_vertexshader_lightmodel.vs", "shaders/EB2_fragmentshader_lightmodel.fs"); //shader para cubo de luz

    // load and create a texture 
    // ----------------------------------------------
    Model ourModelMoon1("model/moon/moon.obj"); //luna
    Model ourModelMario("model/newMario/mario.obj");  //Mario
    Model ourModelLevel1("model/level1/level1.obj"); //Nivel 1 de mario
    Model ourModelMoneda("model/moneda/moneda.obj"); // Monedas
    Model ourModelCameraGuy("model/camera_guy/camera_guy.obj"); //camera guy
    Model ourModelSky1("model/fondo/fondo4.obj");
    Model ourModelSky2("model/fondo/fondo3.obj");
    Model ourModelSky3("model/fondo/fondo2.obj");
    Model ourModelSky4("model/fondo/fondo1.obj");
    Model ourModelMisteryBox("model/misteryBox/misteryBox.obj");
    Model ourModelStar("model/star/star.obj");

    //Enemigos
    Model ourModelPiedra("model/thwomp_super_mario_bros/Thwomp.obj");
    Model ourModelCanon("model/canon/canon.obj");
    Model ourModelBill("model/bullet_bill_super_mario_bros/bill.obj");
    Model ourModelGoomba("model/goomba/goomba.obj"); //Hongo


    // Agrega los modelos al vector
    backgroundModels.push_back(ourModelSky1);
    backgroundModels.push_back(ourModelSky2);
    backgroundModels.push_back(ourModelSky3);
    backgroundModels.push_back(ourModelSky4);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //Dibujar cubos de luz (para el flas de camera guy)
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    // positions of the point lights (estrella y coins )
    glm::vec3 pointLightPositions[] = {
        glm::vec3(-0.714128f, 0.83578f, -9.4153f), //luz de la estrella
        glm::vec3(-0.473441f, 0.691635f, 4.42157f), //luz boxes
        glm::vec3(-0.526412f, 1.13059f, 4.31833f),
        glm::vec3(-0.528894f, 0.752635f, 4.21493f),
        glm::vec3(-0.475071f, 0.758901f, -1.71334f),
        glm::vec3(-0.522486f, 1.20894f, -3.25915f),
        glm::vec3(-0.560055f, 0.713738f, -4.5503f),
        glm::vec3(-0.509445f, 0.709972f, -4.87611f),
        glm::vec3(-0.494378f, 0.675087f, -5.23618f),
        glm::vec3(-0.510319f, 1.209059f, -4.93357f),
        glm::vec3(-0.485098f, 1.205879f, -7.00721f),
        glm::vec3(-0.5113f, 0.796225f, -11.0901f),
        glm::vec3(-0.48482f, 0.793677f, 5.04847f),
        glm::vec3(-2.35357f, 1.60804f, -16.0295f), //flash camera 
    };

    //--------------------------------------------------------------

    // first, configure the cube's lightCubeVAO (and VBO)
    unsigned int VBO, lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightCubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    camera.MovementSpeed = 5; //Optional. Modify the speed of the camera
    float stoneMovementTime = 0.0f;//moviemiento de las piedras
    float amplitudFactor = 0.175f;
    float maxAlturaOffset = 0.2f;

    // Variables de control de la bala
    glm::vec3 bulletPosition;
    bool bulletActive = false;
    float bulletStartTime = 0.0f;

    float billMovementTime = 0.0f;
    float resetTime = 2.0f;


    glm::vec3 marioPos = glm::vec3(0.0f, 0.0f, 0.0f);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        billMovementTime += deltaTime;

        // input
        // -----
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            marioPos.z -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            marioPos.z += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            marioPos.x -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            marioPos.x += 0.01f;


        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f); //brillo modelos

        // --------------directional light (Luz de la luna)
        lightingShader.setVec3("dirLight.direction", 0.0348032, -0.556294, 0.830256); // Dirección de la luz
        lightingShader.setVec3("dirLight.ambient", 0.09f, 0.09f, 0.17f);        // Componente ambiental (azul)
        lightingShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.3f);        // Componente difusa (azul)
        lightingShader.setVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);       // Componente especular (blanco)


        // ---------------Point light
        // Simulando una luz para los pointlights cálida con vectores
        glm::vec3 ambient = glm::vec3(0.1f); // Luz ambiental, 10% de intensidad
        glm::vec3 diffuse = glm::vec3(0.8f); // Luz difusa, 80% de intensidad
        glm::vec3 specular = glm::vec3(0.8f); // Luz especular, 80% de intensidad

        //Luz de la estrella (se prende y se apaga)
        timeEstrella += deltaTime;
        // Calcula el factor de atenuación basado en el tiempo
        float attenuationFactor = 0.5f * (sinf(attenuationSpeed * timeEstrella) + 1.0f); // Esto generará una oscilación suave entre 0 y 1

        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", ambient.x * attenuationFactor, ambient.y * attenuationFactor, ambient.z * attenuationFactor);
        lightingShader.setVec3("pointLights[0].diffuse", 1.0f * attenuationFactor, 1.0f * attenuationFactor, 0.0f * attenuationFactor);
        lightingShader.setVec3("pointLights[0].specular", 1.0f * attenuationFactor, 1.0f * attenuationFactor, 0.0f * attenuationFactor);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.19);
        lightingShader.setFloat("pointLights[0].quadratic", 0.32);

        //Luz de monedas
        for (int i = 1; i < 13; ++i) {
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", ambient.x * 0.1, ambient.y * 0.1, ambient.z * 0.1);
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuse);
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].specular", specular);
            lightingShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            lightingShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.29);
            lightingShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.42);
        }


        //------------------spotLight (camera guy flashlight)
        lightingShader.setVec3("spotLight.position", -2.5663f, 1.61352f, -16.0371f);
        lightingShader.setVec3("spotLight.direction", 0.99877f, -0.0314112f, 0.0383667f);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.19);
        lightingShader.setFloat("spotLight.quadratic", 0.32);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(8.0f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));


        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        //------------------------------- Render de los modelos --------------------------
            //render de loaded model level1 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.9f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        ourModelLevel1.Draw(lightingShader);

        //render de loaded Model Camara_guy 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.5663f, 1.31352f, -16.0371f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setMat4("model", model);
        ourModelCameraGuy.Draw(lightingShader);

        //render de loaded Model MisteryBox 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.530667f, 1.1269f, -16.0179f));
        model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
        lightingShader.setMat4("model", model);
        ourModelMisteryBox.Draw(lightingShader);

        //render de loaded Model Mario
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.514134f+marioPos.x, 0.232393f, 5.45899f+marioPos.z));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        lightingShader.setMat4("model", model);
        ourModelMario.Draw(lightingShader);

        //ENEMIGOS
        //------------render de loaded Model Goomba---------------
       //Movimiento
       // Calcular la posición Z actual usando la función seno
        zOffset = glm::sin(glfwGetTime() * 0.5f) * 0.50238f; // Ajusta la velocidad y amplitud del movimiento
        zPos = 2.99797f + zOffset;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.64199, 0.355299, zPos));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setMat4("model", model);
        ourModelGoomba.Draw(lightingShader);
        //--------------------------------------------
        // Incrementar la variable de tiempo
        stoneMovementTime += deltaTime;

        // Renderizado de la primera piedra
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, 0.65f - (amplitudFactor * sin(stoneMovementTime) + maxAlturaOffset), 1.2f));
        model = glm::scale(model, glm::vec3(0.0001f, 0.0001f, 0.0001f)); // Escala 1000 veces más pequeña
        lightingShader.setMat4("model", model);
        ourModelPiedra.Draw(lightingShader);

        // Renderizado de la segunda piedra
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.3f, 0.65f - (amplitudFactor * sin(stoneMovementTime) + maxAlturaOffset), 1.2f));
        model = glm::scale(model, glm::vec3(0.0001f, 0.0001f, 0.0001f)); // Escala 1000 veces más pequeña
        lightingShader.setMat4("model", model);
        ourModelPiedra.Draw(lightingShader);

        // Renderizado de la tercer piedra a la izquierda del primero
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.7f, 0.65f - (amplitudFactor * sin(stoneMovementTime) + maxAlturaOffset), 1.2f));
        model = glm::scale(model, glm::vec3(0.0001f, 0.0001f, 0.0001f)); // Escala 1000 veces más pequeña
        lightingShader.setMat4("model", model);
        ourModelPiedra.Draw(lightingShader);

        // Renderizado del modelo Canon
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, 0.6f, -7.93f)); // Posición del cañón
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación del cañón (90 grados en el eje Y)
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // Escala del cañón
        lightingShader.setMat4("model", model);
        ourModelCanon.Draw(lightingShader);

        // Mover la bala hacia adelante
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, 0.7f, -7.7f + billMovementTime)); // Posición de la bala
        model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f)); // Escala 1000 veces más pequeña
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación del cañón (90 grados en el eje Y)
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // Escala de la bala
        lightingShader.setMat4("model", model);
        ourModelBill.Draw(lightingShader);

        // Verificar si es el momento de reiniciar la posición
        if (billMovementTime >= resetTime)
        {
            billMovementTime = 0.0f; // Reiniciar el tiempo
        }
        //-----------------------------------------------------------------


        //render de loaded Model Fondo
        //lightingShader.setBool("modelHasSpecular", false);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, -15.0f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));	// it's a bit too big for our scene, so scale it down
        //lightingShader.setMat4("model", model);
        //ourModelSky3.Draw(lightingShader);
        //lightingShader.setBool("modelHasSpecular", true);
        lightingShader.setBool("modelHasSpecular", false);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -15.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
        lightingShader.setMat4("model", model);
        backgroundModels[currentBackgroundIndex].Draw(lightingShader);
        lightingShader.setBool("modelHasSpecular", true);

        //render de loaded Model coin
        for (int i = 1; i < 12; i++) { //12 monedas
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::rotate(model, glm::radians(currentFrame * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            lightingShader.setMat4("model", model);
            ourModelMoneda.Draw(lightingShader);
        }

        // also draw the lamp object(s)
        lightModelShader.use();
        lightModelShader.setMat4("projection", projection);
        lightModelShader.setMat4("view", view);

        // render de loaded Model star 
        lightModelShader.setBool("isStar", true); //Estrella brillosa sin importar la iluminacion ambiente
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.514128f, 0.83578f, -9.4153f));
        model = glm::rotate(model, glm::radians(currentFrame * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre el eje Y
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        lightModelShader.setMat4("model", model);
        ourModelStar.Draw(lightModelShader);

        //render de loaded Model Moon
        lightModelShader.setBool("isStar", false); //Pintar de blanco el modelo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.91134, 12.5643, -28.5863));
        model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5));
        lightModelShader.setMat4("model", model);
        ourModelMoon1.Draw(lightModelShader);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);

        //Flash de camera guy
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[13]);
        model = glm::scale(model, glm::vec3(0.02f));
        lightModelShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    bool isMusicPlaying = false;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!isMusicPlaying) {
            std::cout << "Playing music\n";
            PlaySound(TEXT("C:\\Spider.wav"), NULL, SND_FILENAME | SND_SYNC);
            isMusicPlaying = true;
            std::cout << "Music Over\n";
        }
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


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
