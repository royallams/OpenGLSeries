#include <stdio.h>
#include <string.h>
#include "cmath"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


const GLint WIDTH = 800, HEIGHT= 600;

//VAOS and VBOS for drawing a single triangle


// UNIFORM : - Step 1:- Create a variable in c++ to hold the Offset MOVE value..
GLuint  VAO, VBO, shader, uniformXMove;


// UNIFORM : - Step 2:- Create other  variable in c++ to record the direction and min and max range value for triangle to move.
bool direction = true;
float triOffset  = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;


//Shaders for now we are creating here but usually it would be in another file
// UNIFORM : - Step 3:- In Vertex shader we will create a uniform and add this as an offset to the X position.
// It is a single float uniform.

//Vertex Shader
static const char* vShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
layout (location = 0) in vec3 pos;                                  \n\
                                                                    \n\
uniform float xMove;                                                \n\
void main()                                                         \n\
{                                                                   \n\
        gl_Position = vec4(pos.x+ xMove,pos.y, pos.z, 1.0);    \n\
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
        printf("Error compiling the shader :  '%s'\n", shaderType,eLog);
        return;
    }

    glAttachShader(theProgram, theShader);//Program can have multiple shaders..
}

//Compiling Shaders
void CompileShaders(){
    shader = glCreateProgram();

        if(!shader){
            printf("Error Creating shader program \n");
            return;
        }

     AddShader(shader, vShader, GL_VERTEX_SHADER);
     AddShader(shader, fShader, GL_FRAGMENT_SHADER);


     //Getting the error codes from the shader program
        GLint result = 0 ;
        GLchar eLog[1024] = { 0 };


        glLinkProgram(shader);//Creates an executable in the graphics card
        glGetProgramiv(shader, GL_LINK_STATUS, &result);

        if(!result)
        {
            glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
            printf("Error linking program :  '%s'\n",eLog);
            return;
        }

        glValidateProgram(shader);
        glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

        if(!result)
        {
            glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
            printf("Error Validating program :  '%s'\n",eLog);
            return;
        }

        // UNIFORM : - Step 4:- Lets connect the Shader uniform variable to our c++ variable..
        uniformXMove = glGetUniformLocation(shader, "xMove");

}




int main()
{

    //Initialize GLFW
    if(!glfwInit())
    {
        printf("GLFW Init Failed\n");
        glfwTerminate();
        return 1;
    }

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
        printf("GLFW Mainwindow creation Failed\n");
        glfwTerminate();
        return 1;
    }

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
        printf("GLEW init Failed\n");
        glfwDestroyWindow(mainwindow);// SInce this time we have already created a window.
        glfwTerminate();
        return 1;
    }

    //Setup View port
    glViewport(0,0,bufferWidth, bufferHeight);


    CreateTriangle();
    CompileShaders();

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

        // UNIFORM : - Step 6:- For every looping we will keep updating the c++ uniform variable with the offset we get dynamically.
        // This should automatically update the value to the Vertex Shader..
        // After telling which shader to use .....
        // We will update the value of offset in the shader
        glUniform1f(uniformXMove,triOffset);


        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);


        //Swap betweeen which can be seen and not seen
        glfwSwapBuffers(mainwindow);
    }


    return 0;
}
