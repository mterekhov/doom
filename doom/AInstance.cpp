//
//  AInstance.cpp
//  doom
//
//  Created by Mihail Terekhov on 21.01.2021.
//

#include "AInstance.h"

namespace DoomEngine {

const TCharPointersArray khronosValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

AInstance::AInstance() {
    extensionsList = collectInstanceExtensions();
    extensionsNamesList = collectInstanceExtensionsNames(extensionsList);
    
    uint32_t pApiVersion = 0;
    vkEnumerateInstanceVersion(&pApiVersion);
    
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Doom";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = pApiVersion;
    appInfo.pEngineName = "DoomEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsNamesList.size());
    createInfo.ppEnabledExtensionNames = extensionsNamesList.data();
#ifdef DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = debugMessengerCreateInfo();
    if (checkValidationLayerSupport(khronosValidationLayers)) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(khronosValidationLayers.size());
        createInfo.ppEnabledLayerNames = khronosValidationLayers.data();
        createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debugCreateInfo);
    }
#endif
    
    VkResult error = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
    if (error != VK_SUCCESS) {
        printf("DoomEngine: error creating VULKAN instance\n");
        return;
    }

#ifdef DEBUG
    setupDebugMessenger();
#endif
    printf("DoomEngine: vulkan instance created\n");
}

AInstance::~AInstance() {
//    destroyDebugUtilsMessenger(vulkanInstance, debugMessenger, nullptr);

    vkDestroyInstance(vulkanInstance, nullptr);
    for (const auto& name : extensionsNamesList) {
        delete [] name;
    }
}

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
    printf("DoomEngine: available instance extensions:\n");
    for (const auto& extension : extensionsList) {
        char *newString = new char[256];
        memcpy(newString, extension.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
        namesList.insert(namesList.end(), newString);
    }
    
    return namesList;
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

void AInstance::setupDevice() {
    uint32_t devicesCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &devicesCount, nullptr);
    
    std::vector<VkPhysicalDevice> devicesList(devicesCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &devicesCount, devicesList.data());
    printf("DoomEngine: available devices:\n");
    for (const auto& device : devicesList) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        printf("\t%s\n", deviceProperties.deviceName);
    }
    
}

VKAPI_ATTR VkBool32 VKAPI_CALL AInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
    printf("DoomEngine: %s\n", pCallbackData->pMessage);
    
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

VkDebugUtilsMessengerCreateInfoEXT AInstance::debugMessengerCreateInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    
    return createInfo;
}

void AInstance::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = debugMessengerCreateInfo();
    
    if (createDebugUtilsMessenger(vulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        printf("DoomEngine: failed to set up debug messenger\n");
    }
}

}
