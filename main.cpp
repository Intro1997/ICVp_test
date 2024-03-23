#include "focus.hpp"
#include "gl_window.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  GLFWwindow *window = GetGlWindow(800, 600, "ICVp_Test");
  if (!window) {
    std::cerr << "Create window failed\n";
    return -1;
  }

  RunFocusTest(window);
  return 0;
}