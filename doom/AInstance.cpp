//
//  AInstance.cpp
//  doom
//
//  Created by Mihail Terekhov on 21.01.2021.
//

#include "AInstance.h"

namespace DoomEngine {

static const TCharPointersArray khronosValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

void AInstance::createInstance(void *metalLayer) {
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
    setupLogicalDevice();
    setupSurface(metalLayer);
}

void AInstance::destroyInstance() {
    if (useValidationLayers) {
        destroyDebugUtilsMessenger(vulkanInstance, debugMessenger, nullptr);
    }
    
    vkDestroyDevice(logicalDevice, nullptr);
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
    
    //  Check for shaders support
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
//    if (!deviceFeatures.geometryShader) {
//        return false;
//    }
    
    //  Find support of correct queue for
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamiliesList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiliesList.data());
    int graphicOperationsQueuesNumber = 0;
    for (const auto& queueFamily : queueFamiliesList) {
        if (!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            continue;
        }
        
        graphicOperationsQueuesNumber++;
    }
    
    if (graphicOperationsQueuesNumber == 0) {
        return false;
    }
    
    printf("DoomEngine: using this device %s\n", deviceProperties.deviceName);
    return true;
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
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 3;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        printf("DoomEngine: error creating logical device\n");
    }

    vkGetDeviceQueue(logicalDevice, queueCreateInfo.queueFamilyIndex, 0, &graphicsQueue);
    //    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

}
