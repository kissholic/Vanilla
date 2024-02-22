/*
 * File: graphics.h
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#pragma once

#include <vulkan/vulkan.h>

class graphics_context {
public:
  graphics_context();

private:
  void init_vulkan() noexcept;
  void create_instance() noexcept;
  void create_physical_device() noexcept;

  void setup_debug_messenger() noexcept;

  void cleanup() noexcept;

  bool check_validationlayer_support() noexcept;

private:
  VkInstance m_instance;
  VkPhysicalDevice m_physical_device;
  VkDevice m_device;

  VkDebugUtilsMessengerEXT m_debug_messenger;
};