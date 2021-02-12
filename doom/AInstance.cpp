//
//  AInstance.cpp
//  doom
//
//  Created by Mihail Terekhov on 21.01.2021.
//

#include <iostream>
#include <set>

#include "AInstance.h"

namespace DoomEngine {

static const TCharPointersArray khronosValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const TCharPointersArray deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void AInstance::createInstance(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight) {
    width = frameWidth;
    height = frameHeight;
    
#ifdef DEBUG
    useValidationLayers = true;
#endif
    extensionsList = collectInstanceExtensions();
    extensionsNamesList = collectInstanceExtensionsNames(extensionsList);
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsNamesList.size());
    createInfo.ppEnabledExtensionNames = extensionsNamesList.data();
    
    VkApplicationInfo appInfo = applicationInfo();
    createInfo.pApplicationInfo = &appInfo;
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = appendValidationLayers(createInfo);
    if (useValidationLayers) {
        createInfo.pNext = &debugCreateInfo;
    }
    
    VkResult error = VK_SUCCESS;
    error = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
    if (error != VK_SUCCESS) {
        printf("DoomEngine: error creating VULKAN instance\n");
        return;
    }
    
    if (useValidationLayers) {
        setupDebugMessenger();
    }
    
    setupDevice();
    setupSurface(metalLayer);
    findQueuesIndeces();
    querySwapChainSupport(physicalDevice);
    setupLogicalDevice();
    createSwapChain();
}

void AInstance::destroyInstance() {
    if (useValidationLayers) {
        destroyDebugUtilsMessenger(vulkanInstance, debugMessenger, nullptr);
    }
    
    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
    vkDestroyInstance(vulkanInstance, nullptr);
    
    for (const auto& name : extensionsNamesList) {
        delete [] name;
    }
}

VkApplicationInfo AInstance::applicationInfo() {
    VkApplicationInfo appInfo = {};
    
    uint32_t pApiVersion = 0;
    vkEnumerateInstanceVersion(&pApiVersion);
    appInfo.apiVersion = pApiVersion;
    
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Doom";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "DoomEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    return appInfo;
}

#pragma mark - Swap -

void AInstance::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
    
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        printf("DoomEngine: failed to create swap chain\n");
    }
    
}

VkExtent2D AInstance::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {width, height};

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        
        return actualExtent;
    }
}

SwapChainSupportDetails AInstance::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details = {0};
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    }
    
    return details;
}

VkSurfaceFormatKHR AInstance::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR AInstance::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

#pragma mark - Instance Extensions -

TInstanceExtensionsArray AInstance::collectInstanceExtensions() {
    //  define count of extensions
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    
    //  collect all the extensions
    TInstanceExtensionsArray collectedList(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, collectedList.data());
    
    return collectedList;
}

TCharPointersArray AInstance::collectInstanceExtensionsNames(TInstanceExtensionsArray extensionsList) {
    TCharPointersArray namesList = TCharPointersArray();
    for (const auto& extension : extensionsList) {
        char *newString = new char[256];
        memcpy(newString, extension.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
        namesList.insert(namesList.end(), newString);
        printf("%s\n", newString);
    }
    
    return namesList;
}

#pragma mark - Queues -

void AInstance::findQueuesIndeces() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamiliesList.data());

    for (uint32_t i = 0; i < queueFamiliesList.size(); i++) {
        const auto& queueFamily = queueFamiliesList[i];
        if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            graphicQueueFamilyIndex = i;
        }
        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
            presentQueueFamilyIndex = i;
        }

        if (presentQueueFamilyIndex > 0 && graphicQueueFamilyIndex > 0) {
            return;
        }
    }
}

#pragma mark - Validation Layers -

void AInstance::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = debugMessengerCreateInfo();
    
    if (createDebugUtilsMessenger(vulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        printf("DoomEngine: failed to set up debug messenger\n");
    }
}

bool AInstance::checkValidationLayerSupport(const TCharPointersArray& layersNamesList) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for (const char *layerName : layersNamesList) {
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                return true;
            }
        }
    }
    
    return false;
}

VkDebugUtilsMessengerCreateInfoEXT AInstance::debugMessengerCreateInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    
    return createInfo;
}

VkDebugUtilsMessengerCreateInfoEXT AInstance::appendValidationLayers(VkInstanceCreateInfo& createInfo) {
    if (!checkValidationLayerSupport(khronosValidationLayers)) {
        return VkDebugUtilsMessengerCreateInfoEXT();
    }
    
    createInfo.enabledLayerCount = static_cast<uint32_t>(khronosValidationLayers.size());
    createInfo.ppEnabledLayerNames = khronosValidationLayers.data();
    
    return debugMessengerCreateInfo();
}

VKAPI_ATTR VkBool32 VKAPI_CALL AInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
    printf("DoomEngineLayer: %s\n", pCallbackData->pMessage);
    
    return VK_FALSE;
}

VkResult AInstance::createDebugUtilsMessenger(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void AInstance::destroyDebugUtilsMessenger(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


#pragma mark - Device -

void AInstance::setupDevice() {
    uint32_t devicesCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &devicesCount, nullptr);
    
    std::vector<VkPhysicalDevice> devicesList(devicesCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &devicesCount, devicesList.data());
    for (const auto& device : devicesList) {
        if (checkDeviceCapability(device)) {
            physicalDevice = device;
            return;
        }
    }
}

bool AInstance::checkDeviceCapability(const VkPhysicalDevice& device) {
    //  Find discrete GPU
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        return false;
    }
    
//    //  Check for shaders support
//    VkPhysicalDeviceFeatures deviceFeatures;
//    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
//    if (!deviceFeatures.geometryShader) {
//        return false;
//    }
        
    printf("DoomEngine: using this device %s\n", deviceProperties.deviceName);
    return true;
}

bool AInstance::checkDeviceExtensionSupport(VkPhysicalDevice& device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    
    return requiredExtensions.empty();
}

#pragma mark - Surface -

void AInstance::setupSurface(void *metalLayer) {
    VkMetalSurfaceCreateInfoEXT surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.pLayer = metalLayer;

    if (vkCreateMetalSurfaceEXT(vulkanInstance, &surfaceCreateInfo, NULL, &surface) != VK_SUCCESS) {
        printf("DoomEngine: error creating metal surface\n");
    }
}

#pragma mark - Device -

void AInstance::setupLogicalDevice() {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfosList;
    std::set<int32_t> uniqueQueueFamilies = {graphicQueueFamilyIndex, presentQueueFamilyIndex};
    float queuePriority = 1.0f;
    for (int queueIndex : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfosList.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfosList.size());
    createInfo.pQueueCreateInfos = queueCreateInfosList.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    if (checkDeviceExtensionSupport(physicalDevice)) {
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        printf("DoomEngine: error creating logical device\n");
    }

    vkGetDeviceQueue(logicalDevice, graphicQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, presentQueueFamilyIndex, 0, &presentQueue);
}

}
