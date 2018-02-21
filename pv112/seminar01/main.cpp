#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <istream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Globals
int win_width = 1280;
int win_height = 760;

const int vertices_count = 3;
const float vertices[vertices_count * 2] = {
     0.0f,  0.5f,
    -0.5f, -0.5f,
     0.5f, -0.5f,
};

GLFWwindow *window;

GLuint positions_vbo = 0;
GLuint geometry_vao = 0;
GLuint program = 0;

// Key press callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// Callback function to be called when OpenGL emits an error
void APIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                    const char *message, const void *user_parameter);

// Utility function to load file into string
string load_file_to_string(const char *file_name);

void init();
void resize(GLFWwindow *window, int width, int height);
void render();

int main(void) {
  // Initialize GLFW
  if (!glfwInit()) {
    return -1;
  }

  // Beforce creating the Window, request OpenGL 3.3 Core context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // Create window
  window = glfwCreateWindow(win_width, win_height, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make window's context current
  glfwMakeContextCurrent(window);

  // Load OpenGL functions
  if (!gladLoadGL()) {
    return -1;
  }

  // Not available on MacOS
#ifndef __APPLE__
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(opengl_debug_callback, nullptr);

  glfwSetKeyCallback(window, key_callback);
#endif
  glfwSetWindowSizeCallback(window, resize);

  // Call our init function before render loop
  init();

  // Loop
  while (!glfwWindowShouldClose(window)) {
    // Render
    render();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Free resources
  glDeleteVertexArrays(1, &geometry_vao);
  glDeleteBuffers(1, &positions_vbo);
  glDeleteProgram(program);

  // Free every allocated resource associated with GLFW
  glfwTerminate();

  return 0;
}

void APIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                    const char *message, const void *user_parameter) {
  (void)source;
  (void)id;
  (void)severity;
  (void)length;
  (void)user_parameter;
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    cout << message << endl;
    return;
  default:
    return;
  }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  (void)window;
  (void)scancode;
  (void)action;
  (void)mods;
  switch (key) {
  case GLFW_KEY_L:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case GLFW_KEY_F:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case GLFW_KEY_Q:
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;
  }
}

string load_file_to_string(const char *file_name) {
  ifstream infile{file_name};
  return {istreambuf_iterator<char>(infile), istreambuf_iterator<char>()};
}

void init() {
  // Load and compile shaders
  int compile_status;
  const char *source;

  string vs_source = load_file_to_string("shaders/main.vert");
  GLuint vs_shader = glCreateShader(GL_VERTEX_SHADER);
  source = vs_source.c_str();
  glShaderSource(vs_shader, 1, &source, nullptr);
  glCompileShader(vs_shader);
  glGetShaderiv(vs_shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE) {
    // Retrieve length of the information log
    int log_len = 0;
    glGetShaderiv(vs_shader, GL_INFO_LOG_LENGTH, &log_len);

    // Allocate space for the log and load it
    vector<char> log(log_len);
    glGetShaderInfoLog(vs_shader, log_len, nullptr, log.data());

    cout << "Failed to compile vertex shader" << endl;
    cout << string(begin(log), end(log)) << endl;
  }

  string fs_source = load_file_to_string("shaders/main.frag");
  GLuint fs_shader = glCreateShader(GL_FRAGMENT_SHADER);
  source = fs_source.c_str();
  glShaderSource(fs_shader, 1, &source, nullptr);
  glCompileShader(fs_shader);
  glGetShaderiv(fs_shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE) {
    // Retrieve length of the information log
    int log_len = 0;
    glGetShaderiv(fs_shader, GL_INFO_LOG_LENGTH, &log_len);

    // Allocate space for the log and load it
    vector<char> log(log_len);
    glGetShaderInfoLog(fs_shader, log_len, nullptr, log.data());

    cout << "Failed to compile vertex shader" << endl;
    cout << string(begin(log), end(log)) << endl;
  }

  program = glCreateProgram();
  glAttachShader(program, vs_shader);
  glAttachShader(program, fs_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &compile_status);
  if (compile_status == GL_FALSE) {
    // Retrieve length of the information log
    int log_len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

    // Allocate space for the log and load it
    vector<char> log(log_len);
    glGetProgramInfoLog(program, log_len, nullptr, log.data());

    cout << "Failed to compile vertex shader" << endl;
    cout << string(begin(log), end(log)) << endl;
  }

  glDeleteShader(vs_shader);
  glDeleteShader(fs_shader);

  // Create buffers with geometry
  glGenBuffers(1, &positions_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float) * 2, vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create a vertex array object for the geometry
  glGenVertexArrays(1, &geometry_vao);

  // Set the parameters of the geometry
  glBindVertexArray(geometry_vao);

  // Retrieve index of the "position" attribute in program
  GLuint position_location = glGetAttribLocation(program, "position");
  // Bind geometry buffer to the VAO
  glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
  // Enable the 'position' attribute in shader, linking it with geometry VBO
  glEnableVertexAttribArray(position_location);
  // Define the format of the buffered data (two floats, normalized)
  glVertexAttribPointer(position_location, // index/location
                        2,                 // number of components (1,2,3,4)
                        GL_FLOAT,          // data type of component
                        GL_FALSE,          // should components be normalized?
                        0,                 // stride between attributes
                                           // 0 = tightly packed
                        0                  // offset into the buffer
                        );

  glBindVertexArray(0);

  // Set color of background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void resize(GLFWwindow *window, int width, int height) {
  (void)window;

  win_width = width;
  win_height = height;

  glViewport(0, 0, win_width, win_height);
}

void render() {
  // Fill the framebuffer with background color
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);

  glBindVertexArray(geometry_vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices_count);

  glBindVertexArray(0);
  glUseProgram(0);
}
