#include <stdio.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>



const GLint WIDTH = 800, HEIGHT= 600;



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

    //Loop untill the window is closed.
    while(!glfwWindowShouldClose(mainwindow))
    {
        //Get and handle user input events..
        glfwPollEvents();

        //Clear Window
        glClearColor(1.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Swap betweeen which can be seen and not seen
        glfwSwapBuffers(mainwindow);
    }


    return 0;
}
