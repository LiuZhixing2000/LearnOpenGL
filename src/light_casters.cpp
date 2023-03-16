#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_class.h"
#include "camera_class.h"
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
# define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
camera_ camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

// timeing
float delta_time = 0.0f;
float last_frame = 0.0f;

// light position
glm::vec3 light_pos(1.2f, 1.0f, 2.0f);


// functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int load_texture(const char* pic_path);

int main(){
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting Maps", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to creat GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGl function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shader programs
    // ---------------------------------
    shader_ cube_shader("./shaders/vertex/cube.vs", "./shaders/fragment/cube.fs");
    shader_ light_shader("./shaders/vertex/light.vs", "./shaders/fragment/light.fs");

    // set up vertex data and configure vertex attributes
    // --------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // vertex buffer object, VBO
    // vertex array object, VAO
    unsigned int vbo, cube_vao, light_vao;
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &vbo);
    // bind and set vertex buffer and element buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // then bind the vertex array object
    glBindVertexArray(cube_vao);
    // and then configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // generate texture
    unsigned int diff_texture_id = load_texture("./pictures/container2.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff_texture_id);
    unsigned int spec_texture_id = load_texture("./pictures/container2_specular.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec_texture_id);

    cube_shader.use();
    cube_shader.set_int("material.diffuse", 0);
    cube_shader.set_int("material.specular", 1);

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window)){
        // per-frame time logic
        // --------------------
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        // -----
        processInput(window);    

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // shading cube
        cube_shader.use();

        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        cube_shader.set_mat4("projection", projection);
        // view matrix
        glm::mat4 view = camera.get_view_matrix();
        cube_shader.set_mat4("view", view);

        cube_shader.set_vec3("view_pos", camera.position_.x, camera.position_.y, camera.position_.z);

        // cube_shader.set_vec3("material.specular", 0.5f, 0.5f, 0.5f);
        cube_shader.set_float("material.shiness", 64.0f);

        cube_shader.set_vec3("light.pos", camera.position_);
        cube_shader.set_float("light.constant", 1.0f);
        cube_shader.set_float("light.linear", 0.09f);
        cube_shader.set_float("light.quadratic", 0.032f);
        cube_shader.set_vec3("light.direction", camera.front_);
        cube_shader.set_float("light.cut_off", glm::cos(glm::radians(12.5f)));
        cube_shader.set_float("light.outer_cut_off", glm::cos(glm::radians(15.0f)));
        cube_shader.set_vec3("light.ambient", 0.2f, 0.2f, 0.2f);
        cube_shader.set_vec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        cube_shader.set_vec3("light.specular", 1.0f, 1.0f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diff_texture_id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, spec_texture_id);

        glBindVertexArray(cube_vao);
        for(unsigned int i = 0; i < 10; i++){
            glm::mat4 model(1.0);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cube_shader.set_mat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // // shading light
        // light_shader.use();
        // // model matrix
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, light_pos);
        // model = glm::scale(model, glm::vec3(0.2f));
        // light_shader.set_mat4("model", model);
        // // view matrix
        // light_shader.set_mat4("view", view);
        // // projection matrix
        // light_shader.set_mat4("projection", projection);

        // glBindVertexArray(light_vao);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_DEPTH_TEST);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // optional: de-allocate all resources onve they've outlived their purpose
    // -----------------------------------------------------------------------
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteVertexArrays(1, &light_vao);
    glDeleteBuffers(1, &vbo);
    cube_shader.delete_shader();
    light_shader.delete_shader();
    // glDeleteProgram(shader_program);

    // glfw:terminate, clearing all previously allocated GLFW resources.
    // -----------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, delta_time);
}

// glfw: whenever the mouse moves
// ------------------------------
void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in){
    float xpos = static_cast<float>(xpos_in);
    float ypos = static_cast<float>(ypos_in);
    if(first_mouse){
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x, yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    camera.process_mouse_movement(xoffset, yoffset);
}

// glfw: whenever the mouse scrolls
// --------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.process_mouse_scroll(static_cast<float>(yoffset));
}

unsigned int load_texture(const char* pic_path){
    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nr_components;
    unsigned char* data = stbi_load(pic_path, &width, &height, &nr_components, 0);
    if(data){
        GLenum format;
        if(nr_components == 1)
            format = GL_RED;
        else if(nr_components == 3)
            format = GL_RGB;
        else if(nr_components == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    return texture_id;
}