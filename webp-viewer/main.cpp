#include <iostream>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
     glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv)
{
  if (!glfwInit())
  {
    std::cerr << "Initialization of GLFW failed!\n";
    return 1;
  }

  GLFWwindow * window = glfwCreateWindow(640, 480, "webp viewer", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    std::cerr << "Window creation failed!\n";
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  std::cout << "It's the end of the program." << std::endl;
  return 0;
}
