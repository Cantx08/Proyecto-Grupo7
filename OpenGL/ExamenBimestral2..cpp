#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(const char *path);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// variacion de posicion
 //variación de posicion en x del villano
    float offset = 1.0f; // Posicion inicial del villano
     float offset1 = -5.0f; // Posicion inicial del villano
    float direction = 1.0f;
// Velocidad de Rotacion para el Pozo 
   float VelocidadRota = 4.0f;

// texture
//float visibilyFactor;

// Luz 
//glm::vec3 lightState(1.0f);



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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Escena Mario Bros", NULL, NULL);
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

    // build and compile shaders
    // -------------------------

    // Shader a usar general
    Shader ourShader("shaders/shader_exercise_mloading.vs", "shaders/shader_exercise_mloading.fs");
    // Shader para cargar model cielo 
    Shader ourModelShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");
    
   
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // -----------------------------------------------------------------
    // load and create a texture 
    // ---------------------
    //Model ourModelMario("model/mario/mario.obj");  // load the model mario.obj
    Model ourModelMario("model/mario/mario.obj");  // load the model mario.obj
    Model ourModelVillano("model/villano/villano.obj"); // load the model villano.obj
    Model ourModelPozo("model/pozo/pozo.obj");  // load the model pozo.obj
    Model ourModelRecompensa("model/recompensa/recompensa.obj"); // load the model recompensa.obj
    Model ourModelPasillo("model/pasillo/pasillo.obj"); // load the model pasillo.obj   
    
    // Cargar modelos que se seran iluminados (Monedas) 
    Model ourModelMoneda("model/moneda/moneda.obj"); 
    //Cargar modelo Camara_guy
    Model ourModelCamara("model/camera_guy/camera_guy.obj"); 

    // Cargar modelo para el suelo 
    Model ourModelLava("model/lava/lava.obj");

    //Cargar modelo sky 
    Model ourModelSky("model/sky/sky.obj");

    //Cargar model moon
    Model ourModelMoon("model/Moon/moon.obj");

    // Cargar modelos que tendran Luz 
   // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // Posiciones espaciales de los point lights
glm::vec3 pointLightPositions[] = {
    //Monedas 
    glm::vec3(-0.5f, -0.6f, -1.0f),
    glm::vec3(-0.5f, -0.4f, -10.0f),
    glm::vec3(-0.5f, -0.6f, -4.9f),
    glm::vec3(-0.5f, -0.4f, -0.7f),
    glm::vec3(-0.5f, -0.8f, -0.9f),
    glm::vec3(-0.5f, -0.3f, -13.5f),
    glm::vec3(-0.5f, -0.8f, -7.0f)
   
};

//Las tonalidades de cada uno de los puntos de luz de la escena. 
    glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 1.0f, 0.0f), //Amarillo
    glm::vec3(1.0f, 1.0f, 0.0f), //Amarillo
    glm::vec3(1.0f, 1.0f, 0.0f), //Amarillo
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
   
};


    camera.MovementSpeed = 10; //Optional. Modify the speed of the camera

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
      //  glClearColor(0.588f, 0.761f, 0.85f, 1.0f);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

   
        ourShader.setVec3("viewPos", camera.Position);

      
        ourShader.setFloat("material.shininess", 32.0f);
        float radius = 3.5f;
        float camX = sin(glfwGetTime()*0.05) * radius;
        float camZ = cos(glfwGetTime()*0.05) * radius;

       
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        
        //Dibujar los Point Lights sobre moneditas
            for (int i = 0; i < 7; i++) {
                ourShader.setVec3("pointLights[" + to_string(i) + "].position", glm::vec3(-20.0f, 7.2f, i));
                ourShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.5f, 0.5f, 0.0f);
                ourShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 5.0f, 5.0f, 0.0f); // Color amarillo 
                ourShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
                ourShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
                ourShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.09);
                ourShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032);
            
        }



        ourShader.setVec3("spotLight.position", glm::vec3(-1.5f, 1.4f, -0.0f));
        ourShader.setVec3("spotLight.direction", glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setVec3("spotLight.ambient", 0.3f, 0.3f, 0.3f);
        ourShader.setVec3("spotLight.diffuse", 3.0f, 3.0f, 3.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.014);
        ourShader.setFloat("spotLight.quadratic", 0.0007);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.0f)));
      
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
                
        

        
        glm::mat4 model = glm::mat4(1.0f); // Definicion de Model
        
        //Pozo
        model = glm::mat4(1.0f);
        float angle = glfwGetTime() * VelocidadRota;
        model = glm::translate(model, glm::vec3(-0.5f, 0.1f, -0.6f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        model = glm::rotate(model, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);
        ourModelPozo.Draw(ourShader);

       //render de loaded Model Pasillo 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.9f, -0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModelPasillo.Draw(ourShader);
       
        // Dibujar Moneditas 
        for (int i = 0; i < 11; i++) {
            //Se utiliza un for para crear monedas  en toda la escena 
            model = glm::mat4(1.0f);
            float angleCoin = glfwGetTime() * 50.0f; //Para hacer que la moneda rote a traves del tiempo
            model = glm::translate(model, glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y + 1.8f, pointLightPositions[i].z));
            //se hace un translate en cada uno de las posiciones de los puntos de luz, es decir, las mondenas parecen brillan 
            model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// Es un poco demasiado pequeño para la escena,entonces se lo aumenta 
            model = glm::rotate(model, glm::radians(angleCoin), glm::vec3(0.0f, -1.0f, 0.0f)); //Para que las monedas roten en eje y 
            ourShader.setMat4("model", model);
            ourModelMoneda.Draw(ourShader);
        } 

        
          //render de loaded Model Camara_guy 
        model = glm::mat4(1.0f);
       // model = glm::translate(model,camera.Position-glm::vec3(0.0f,0.0f,1.0f)); // translate it down so it's at the center of the scene
        model = glm::translate(model, glm::vec3(-1.5f, 1.4f, -14.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Para que las monedas roten en eje y 
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Para que las monedas roten en eje x 
        
        ourShader.setMat4("model", model);
        ourModelCamara.Draw(ourShader);

        //Render de model Villano 1 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 0.15f, offset)); 
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Para que las monedas roten en eje x 
        
        
        //Movimiento de Traslacion del elemenento en el eje X
        offset = offset + direction*0.006f;
        if (offset > 1.5 || offset < 0.7f) {

            direction = -direction;
        }

        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModelVillano.Draw(ourShader);

        //Render de model Villano 2 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 0.15f, offset1)); 
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Para que las monedas roten en eje x 
        
        
        //Movimiento de Traslacion del elemenento en el eje X
        offset1 = offset1 + direction*0.006f;
        if (offset < -7.5 || offset1 > -4.7f) {

            direction = -direction;
        }

        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModelVillano.Draw(ourShader);


        // Dibujar el suelo de la escena 

        model = glm::mat4(1.0f);
       // model = glm::translate(model,camera.Position-glm::vec3(0.0f,0.0f,1.0f)); // translate it down so it's at the center of the scene
        model = glm::translate(model, glm::vec3(0.0f, -3.4f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModelLava.Draw(ourShader);


        model = glm::mat4(1.0f);
       // model = glm::translate(model,camera.Position-glm::vec3(0.0f,0.0f,1.0f)); // translate it down so it's at the center of the scene
        model = glm::translate(model, glm::vec3(-0.4f, 0.15f, 4.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Para que las monedas roten en eje x 
        ourShader.setMat4("model", model);
        ourModelMario.Draw(ourShader);

        ourModelShader.use();
        ourModelShader.setMat4("projection", projection);
        ourModelShader.setMat4("view", view);
        //render de loaded Model Sky
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f,-0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourModelShader.setMat4("model", model);
        ourModelSky.Draw(ourModelShader);
         

        


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
void processInput(GLFWwindow *window)
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