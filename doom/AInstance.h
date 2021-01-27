//
//  AInstance.hpp
//  doom
//
//  Created by Mihail Terekhov on 21.01.2021.
//

#ifndef AInstance_hpp
#define AInstance_hpp

#include <stdio.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace DoomEngine {

typedef std::vector<VkExtensionProperties> TInstanceExtensionsArray;
typedef std::vector<std::string> TStringsArray;
typedef std::vector<const char *> TCharPointersArray;

class AInstance {
public:
    AInstance();
    ~AInstance();
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData);
private:
    bool useValidationLayers = false;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance vulkanInstance;
    VkPhysicalDevice device;
    TInstanceExtensionsArray extensionsList;
    TCharPointersArray extensionsNamesList;
    
    VkDebugUtilsMessengerCreateInfoEXT appendValidationLayers(VkInstanceCreateInfo& createInfo);
    TInstanceExtensionsArray collectInstanceExtensions();
    TCharPointersArray collectInstanceExtensionsNames(TInstanceExtensionsArray extensionsList);
    bool checkValidationLayerSupport(const TCharPointersArray& layersNamesList);
    
    void setupDevice();

    void setupDebugMessenger();
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo();
    VkResult createDebugUtilsMessenger(VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkDebugUtilsMessengerEXT* pDebugMessenger);
    void destroyDebugUtilsMessenger(VkInstance instance,
                                    VkDebugUtilsMessengerEXT debugMessenger,
                                    const VkAllocationCallbacks* pAllocator);
};

}

#endif /* AInstance_hpp */
