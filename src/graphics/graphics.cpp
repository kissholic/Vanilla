/*
 * File: graphics.cpp
 * Author: kissholic
 * Copyright (c) 2024 kissholic. All wrongs reserved.
 */

#include "graphics.h"
#include <GLFW/glfw3.h>
#include <map>
#include <spdlog/spdlog.h>
#include <vector>

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
    VkAllocationCallbacks const *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) noexcept {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

std::vector<const char *> getRequiredExtensions() noexcept {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  spdlog::warn("validation layer: %s", pCallbackData->pMessage);
  return VK_FALSE;
}

void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) noexcept {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
}

bool is_device_suitable(VkPhysicalDevice &device) noexcept {
  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceProperties(device, &device_properties);
  vkGetPhysicalDeviceFeatures(device, &device_features);

  return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
         device_features.geometryShader;
}

void graphics_context::setup_debug_messenger() noexcept {
  if (!enableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr,
                                   &m_debug_messenger) != VK_SUCCESS) {
    spdlog::error("failed to set up debug messenger.");
    std::terminate();
  }
}

void graphics_context::init_vulkan() noexcept {
  create_instance();
  setup_debug_messenger();
}

void graphics_context::create_instance() noexcept {
  if (enableValidationLayers && !check_validationlayer_support()) {
    spdlog::error("validation layers requested, but not available.");
    std::terminate();
  }

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Vanilla";
  app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;
  app_info.pEngineName = "Vanilla Engine";
  app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);

  VkInstanceCreateInfo instance_info{};
  instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_info.pApplicationInfo = &app_info;

  uint32_t extension_count = 0;
  char const **extensions;
  extensions = glfwGetRequiredInstanceExtensions(&extension_count);

  std::vector<char const *> required_extensions;
  for (uint32_t i = 0; i < extension_count; ++i) {
    required_extensions.emplace_back(extensions[i]);
  }
  required_extensions.emplace_back(
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  instance_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  instance_info.enabledExtensionCount = (uint32_t)required_extensions.size();
  instance_info.ppEnabledExtensionNames = required_extensions.data();

  instance_info.enabledLayerCount = 0;
  if (vkCreateInstance(&instance_info, nullptr, &m_instance) != VK_SUCCESS) {
    spdlog::error("Failed to create vulkan instance.");
    std::terminate();
  }
}

void graphics_context::create_physical_device() noexcept {
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

  if (device_count == 0) {
    spdlog::error("failed to find GPU supporting vulkan.");
    std::terminate();
  }

  std::vector<VkPhysicalDevice> devices{device_count};
  vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

  std::multimap<int, VkPhysicalDevice> cadidates;

  for (auto &device : devices) {
    if (is_device_suitable(device)) {
      physicalDevice = device;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    spdlog::error("failed to find a suitable GPU.");
    std::terminate();
  }
}

void graphics_context::cleanup() noexcept {
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
  }
  vkDestroyInstance(m_instance, nullptr);
}

bool graphics_context::check_validationlayer_support() noexcept {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}