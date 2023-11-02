#include <iostream>
#include <glad/glad.h>
#include "include/GLFW/glfw3.h"

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
    int succes;
    char infoLog[512];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL first program", NULL, NULL);
    if(window == NULL){ // window not created
        std::cout << "Could not create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialized GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("src/assets/vertex_core.glsl", "src/assets/fragment_core.glsl");
    Shader shader2("src/assets/vertex_core.glsl", "src/assets/fragment_core2.glsl");

    //vertex array
    float vertices[] = {
        //positions             colors
        -0.5f, -0.5f, 0.0f,	    1.0f, 1.0f, 0.5f,	// bottom left
		-0.5f, 0.5f, 0.0f,	    0.5f, 1.0f, 0.75f,	// top left
		0.5f, -0.5f, 0.0f,	    0.6f, 1.0f, 0.2f,	// bottom right
		0.5f, 0.5f, 0.0f,	    1.0f, 0.2f, 1.0f	// top right
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
		3, 1, 2  // second triangle
    };

    //VAO, VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind VAO
    glBindVertexArray(VAO);

    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set attribute pointer

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.activate();
    shader.setMat4("transform", trans);

    glm::mat4 trans2 = glm::mat4(1.0f);
    trans2 = glm::scale(trans2, glm::vec3(1.5f));
    trans2 = glm::rotate(trans2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader2.activate();
    shader2.setMat4("transform", trans);

    while(!glfwWindowShouldClose(window)){
        //send new frame to window
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.activate();
        shader.setMat4("transform",trans);

        //draw shapes
        glBindVertexArray(VAO);
        shader.activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader2.activate();
        shader2.setMat4("transform", trans2);

        shader2.activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();      
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
