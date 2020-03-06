#include <stdio.h>
#include <string.h>
#include "cmath"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Including Open GL maths Library.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>// This will help us give data to shaders with raw data.



const GLint WIDTH = 800, HEIGHT= 600;

//Step 1:- Create a radian for the rotation (basically convert the angle into radian)
const float toRadians = 3.14159265f / 180.0f ;

GLuint  VAO, VBO, shader, uniformModel;//New variable uniformmodel..


bool direction = true;
float triOffset  = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;


//Demo 1:-
float curAngle = 0.0f;


/*Some distortion may happen because of which we will use *0.4 in the shader,However once we have scaling this may not be needed.
 *If we enable even translation here then it would be doing both .
 * Experiment with both by enabling and disabling..
 * eg :- Rotate then translate  or Translate first then rotate , build and see the result..
 *
 */

//Vertex Shader
static const char* vShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
layout (location = 0) in vec3 pos;                                  \n\
                                                                    \n\
uniform mat4 model;                                                 \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
        gl_Position = model * vec4(pos.x*0.4,pos.y*0.4, pos.z, 1.0);    \n\
}";


//Fragment Shader
static const char* fShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
out vec4 colour;                                                    \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
        colour = vec4(1.0, 1.0, 0.0, 1.0);    \n\
}";


void CreateTriangle()
{

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);//Binding buffer.

    glBindVertexArray(0);//This is unbinding the previously bind array.

    std::cout<<"----------------Triangle Created."<<std::endl;


}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{

    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);


    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    //Get the error for compilation
    GLint result = 0 ;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if(!result)
    {
        glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout<<"Error compiling the shader :  "<< shaderType<< eLog <<std::endl;
        return;
    }

    glAttachShader(theProgram, theShader);//Program can have multiple shaders..
    std::cout<<"----------------Shader Added"<<std::endl;

}

void CompileShaders(){

    shader = glCreateProgram();

        if(!shader){
            std::cout<<"1) Creating Shader Program : FAILED"<<std::endl;
            return;
        }

     std::cout<<"--------a) Creating Shader Program : SUCCESS"<<std::endl;
     std::cout<<"--------b) VERTEX SHADER "<<std::endl;
     AddShader(shader, vShader, GL_VERTEX_SHADER);
     std::cout<<"--------c) FRAGMENT SHADER "<<std::endl;
     AddShader(shader, fShader, GL_FRAGMENT_SHADER);


     //Getting the error codes from the shader program
        GLint result = 0 ;
        GLchar eLog[1024] = { 0 };


        glLinkProgram(shader);//Creates an executable in the graphics card
        glGetProgramiv(shader, GL_LINK_STATUS, &result);

        if(!result)
        {
            glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
            std::cout<<"--------d) Linking/Creating graphics EXES : FAILED  "<<eLog<<std::endl;
            return;
        }
        std::cout<<"--------d) Linking/Creating graphics EXES : SUCCESS "<<std::endl;

        glValidateProgram(shader);
        glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

        if(!result)
        {
            glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
            std::cout<<"--------e) Validation : FAILED "<< eLog<< std::endl;
            return;
        }

        std::cout<<"--------e) Validation : SUCCESS "<<std::endl;

        // UNIFORM : - Step 4:- Lets connect the Shader uniform variable to our c++ variable..
        uniformModel = glGetUniformLocation(shader, "model");

}




int main()
{

    if(!glfwInit())
    {
        std::cout<<"Step 1: GLFW Init Failed"<<std::endl;
        glfwTerminate();
        return 1;
    }

    std::cout<<"Step 1: GLFW Init Success"<<std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainwindow = glfwCreateWindow(WIDTH,HEIGHT,"Test Window", NULL, NULL);
    if(!mainwindow)
    {
        std::cout<<"Step 2: GLFW Mainwindow creation Failed"<<std::endl;
        glfwTerminate();
        return 1;
    }
    std::cout<<"Step 2: GLFW Mainwindow creation Success"<<std::endl;

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainwindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainwindow);// We can keep changing this window


    glewExperimental = GL_TRUE;


    if(glewInit() != GLEW_OK)
    {
        std::cout<<"GLEW init Failed. So destroying GLFW context.."<<std::endl;
        glfwDestroyWindow(mainwindow);// SInce this time we have already created a window.
        glfwTerminate();
        return 1;
    }

    glViewport(0,0,bufferWidth, bufferHeight);

    std::cout<<"Step 3: Creating a triangle."<<std::endl;
    CreateTriangle();
    std::cout<<"Step 4: Compiling Shaders"<<std::endl;
    CompileShaders();

    std::cout<<"Step 5: Window loop.."<<std::endl;

    while(!glfwWindowShouldClose(mainwindow))
    {
        glfwPollEvents();

        if(direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if(std::abs(triOffset) >= triMaxOffset)//abs is a standard library.
        {
            direction = ! direction;
        }


        curAngle += 0.1f;
        if(curAngle >= 360)
        {
            curAngle -= 360;
        }



        glClearColor(0.0f,0.0f,0.0f,1.0f);//Setting it to black.
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shader);



        // IMPO1 : - the order of trans and rot is very important..

        // IMPO2 :-  The traingle may not be contend in the screen currently because of world coordinates.
                    // We need to convert the world coordinates in to screen coordinates for proper visibility.

        glm::mat4 model;// only diagonal is 1 rest all 0
        model = glm::translate(model,glm::vec3(triOffset, 0.0f,0.0f));// Only changing the X values .Applying translation.

        //Step 2:- We dont have to do much changes . Directly use roatation function.45 degree we will convert it into radian
        model = glm::rotate(model, curAngle*toRadians, glm::vec3(0.0f,0.0f,1.0f));

        glUniformMatrix4fv(uniformModel,1, GL_FALSE, glm::value_ptr(model));


        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);


        glfwSwapBuffers(mainwindow);
    }


    return 0;
}
