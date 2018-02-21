#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Window {
public:
  Window(int initial_width, int initial_height, const std::string &title);
  ~Window();

  void set_user_pointer(void *pointer);

  void set_pos_callback(GLFWwindowposfun function);
  void set_size_callback(GLFWwindowsizefun function);
  void set_close_callback(GLFWwindowclosefun function);

  void set_key_callback(GLFWkeyfun function);
  void set_mouse_position_callback(GLFWcursorposfun function);
  void set_mouse_button_callback(GLFWmousebuttonfun function);

  bool should_close();
  void swap_buffers();
  void poll_events();

  void resize(int width, int height);

  int get_width() { return width; }
  int get_height() { return height; }

private:
  int width;
  int height;

  GLFWwindow *window;
};
