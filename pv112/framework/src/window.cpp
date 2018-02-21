#include "window.hpp"

Window::Window(int initial_width, int initial_height, const std::string &title)
    : width(initial_width), height(initial_height) {
  // Beforce creating the Window, request OpenGL 3.3 Core context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  this->window = glfwCreateWindow(initial_width, initial_height, title.c_str(), nullptr, nullptr);

  if (!this->window) {
    glfwTerminate();
    throw "Could not create window!";
  }

  // Make window's context current
  glfwMakeContextCurrent(this->window);

  // Load OpenGL functions
  if (!gladLoadGL()) {
    throw "Could not initialize OpenGL functions!";
  }
}

void Window::set_user_pointer(void *pointer) { glfwSetWindowUserPointer(this->window, pointer); }

void Window::set_pos_callback(GLFWwindowposfun function) { glfwSetWindowPosCallback(this->window, function); }

void Window::set_size_callback(GLFWwindowsizefun function) { glfwSetWindowSizeCallback(this->window, function); }

void Window::set_close_callback(GLFWwindowclosefun function) { glfwSetWindowCloseCallback(this->window, function); }

void Window::set_key_callback(GLFWkeyfun function) { glfwSetKeyCallback(this->window, function); }
void Window::set_mouse_position_callback(GLFWcursorposfun function) {
  glfwSetCursorPosCallback(this->window, function);
}
void Window::set_mouse_button_callback(GLFWmousebuttonfun function) {
  glfwSetMouseButtonCallback(this->window, function);
}

bool Window::should_close() { return glfwWindowShouldClose(this->window) == 0 ? false : true; }

void Window::swap_buffers() { glfwSwapBuffers(this->window); }

void Window::poll_events() { glfwPollEvents(); }

void Window::resize(int width, int height) {
  this->width = width;
  this->height = height;
}

Window::~Window() { glfwDestroyWindow(window); }
