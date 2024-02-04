/*
 * File: graphic_lib.h
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#pragma once

class graphic_lib {
public:
  graphic_lib() { init_glfw(); }

  ~graphic_lib() { fin_glfw(); }

  bool init_glfw();
  void fin_glfw();
};
