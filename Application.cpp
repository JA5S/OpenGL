#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "GLErrorManager.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //set window refresh rate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //vertex positions and textures
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,//0
             0.5f, -0.5f, 1.0f, 0.0f,//1
             0.5f,  0.5f, 1.0f, 1.0f,//2
            -0.5f,  0.5f, 0.0f, 1.0f //3
        };

        unsigned int indices[]{
            0, 1, 2,
            2, 3, 0,
        };

        //blender
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va; //vertex array object (vao)
        VertexBuffer vb(vertices, 4 * 4 * sizeof(float)); //vertex buffer

        //provide specification of the vertices (tells gpu how to read the provided info)
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        //add buffer and layout to vertex array
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6); //index buffer

        //projection matrix
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f); //first 4 floats are window boundaries

        //create and bind shader
        Shader shader("Basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 0.2f, 0.8f, 0.4f, 1.0f); //set rgba vec4 in fragment shader  //no longer using u_color
        shader.SetUniformMat4f("u_MVP", proj);

        //texture
        Texture texture("Sword.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.2f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //update color
            shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.8f, 0.4f, 1.0f);  //no longer using u_color

            //draw call for triangles
            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}