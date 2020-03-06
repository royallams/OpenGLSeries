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


//Three coordinate system :-
//Model coordinates :- Origin to the actual world coordinates.
// We will be using model matrix.
//Since we are not using proper perspective projection ..out triangle remains in same size even if we move from left to right.





const GLint WIDTH = 800, HEIGHT= 600;

//VAOS and VBOS for drawing a single triangle


// UNIFORM : - Step 1:- Create a variable in c++ to hold the Offset MOVE value..
GLuint  VAO, VBO, shader, uniformModel;//New variable uniformmodel..


// UNIFORM : - Step 2:- Create other  variable in c++ to record the direction and min and max range value for triangle to move.
bool direction = true;
float triOffset  = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;


//Shaders for now we are creating here but usually it would be in another file
// UNIFORM : - Step 3:- In Vertex shader we will create a uniform and add this as an offset to the X position.
// It is a single float uniform.



// Now here instead of float we will be using matrix variable
// and instead of offset adding we will multiply the whole position by the transformed model matrix value.
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
        gl_Position = model * vec4(pos.x,pos.y, pos.z, 1.0);    \n\
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

        //Drawing Yellow through shaders.

void CreateTriangle()
{

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    //Step 1:- Create a VAO ....vertex arrays
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Step 2:- Create a VBO ..vertex buffers.
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

//Compiling Shaders
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

    //Initialize GLFW
    if(!glfwInit())
    {
        std::cout<<"Step 1: GLFW Init Failed"<<std::endl;
        glfwTerminate();
        return 1;
    }

    std::cout<<"Step 1: GLFW Init Success"<<std::endl;

    //Setup GLFW Window properties.
    //OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Core Profile ..for backward
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //or forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Creating a window..we will create by function and not new op.
    GLFWwindow *mainwindow = glfwCreateWindow(WIDTH,HEIGHT,"Test Window", NULL, NULL);
    if(!mainwindow)
    {
        std::cout<<"Step 2: GLFW Mainwindow creation Failed"<<std::endl;
        glfwTerminate();
        return 1;
    }
    std::cout<<"Step 2: GLFW Mainwindow creation Success"<<std::endl;

    //Get Buffer Information...may be jus to find the centre of the window..
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainwindow, &bufferWidth, &bufferHeight);

    //Set context for GLFW  to Use..meaning hey GLFW use this window to display your openGL stuffs.
    glfwMakeContextCurrent(mainwindow);// We can keep changing this window


    // Incase you want to have modern features to try out ..
    glewExperimental = GL_TRUE;

    // Now when things are ready for window
    // We will proceed to GLEW...portion..

    if(glewInit() != GLEW_OK)
    {
        std::cout<<"GLEW init Failed. So destroying GLFW context.."<<std::endl;
        glfwDestroyWindow(mainwindow);// SInce this time we have already created a window.
        glfwTerminate();
        return 1;
    }

    //Setup View port
    glViewport(0,0,bufferWidth, bufferHeight);

    std::cout<<"Step 3: Creating a triangle."<<std::endl;
    CreateTriangle();
    std::cout<<"Step 4: Compiling Shaders"<<std::endl;
    CompileShaders();

    std::cout<<"Step 5: Window loop.."<<std::endl;

    //Loop untill the window is closed.
    while(!glfwWindowShouldClose(mainwindow))
    {
        //Get and handle user input events..
        glfwPollEvents();


        // UNIFORM : - Step 5:- Lets update our c++ variable ...like direction, offsets ..
        // if movement reaches max then turn the opposite
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



        //Clear Window
        glClearColor(0.0f,0.0f,0.0f,1.0f);//Setting it to black.
        glClear(GL_COLOR_BUFFER_BIT);


        //Using the Shaders
        glUseProgram(shader);


        glm::mat4 model;// only diagonal is 1 rest all 0
        model = glm::translate(model,glm::vec3(triOffset, 0.0f,0.0f));// Only changing the X values .Applying translation.


        // UNIFORM : - Step 6:- For every looping we will keep updating the c++ uniform variable with the offset we get dynamically.
        // This should automatically update the value to the Vertex Shader..
        // After telling which shader to use .....
        // We will update the value of offset in the shader
//        glUniform1f(uniformXMove,triOffset); //INSTEAD OF THESE WE WILL USE MATRIX DATA NOW
        glUniformMatrix4fv(uniformModel,1, GL_FALSE, glm::value_ptr(model));


        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);


        //Swap betweeen which can be seen and not seen
        glfwSwapBuffers(mainwindow);
    }


    return 0;
}
