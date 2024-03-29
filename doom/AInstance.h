#ifndef SPCDOOMENGINE_AINSTANCE_H
#define SPCDOOMENGINE_AINSTANCE_H

#include <stdio.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace spcDoomEngine {

typedef std::vector<VkExtensionProperties> TInstanceExtensionsArray;
typedef std::vector<std::string> TStringsArray;
typedef std::vector<const char *> TCharPointersArray;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class AInstance {
public:
    void createInstance(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight);
    void destroyInstance();
    void drawFrame();

private:
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

    std::vector<VkCommandBuffer> commandBuffers;
    VkCommandPool commandPool;
    
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    VkInstance vulkanInstance;
    VkSurfaceKHR surface;
    
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    
    TInstanceExtensionsArray extensionsList;
    TCharPointersArray extensionsNamesList;
    VkDebugUtilsMessengerEXT debugMessenger;
    bool useValidationLayers = false;
    int32_t graphicQueueFamilyIndex = -1;
    int32_t presentQueueFamilyIndex = -1;
    uint32_t width = 0;
    uint32_t height = 0;

    void createSemaphores();

    void createCommandPool();
    void createCommandBuffers();
    
    void createRenderPass();
    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<uint8_t>& code);

    void createFramebuffers();
    void createSwapChain();
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void createImageViews();

    void setupDevice();
    bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
    bool checkDeviceCapability(const VkPhysicalDevice& device);
    
    void setupLogicalDevice();

    void setupSurface(void *metalLayer);
    
    void findQueuesIndeces();

    VkDebugUtilsMessengerCreateInfoEXT appendValidationLayers(VkInstanceCreateInfo& createInfo);
    TInstanceExtensionsArray collectInstanceExtensions();
    TCharPointersArray collectInstanceExtensionsNames(TInstanceExtensionsArray extensionsList);
    bool checkValidationLayerSupport(const TCharPointersArray& layersNamesList);
    VkApplicationInfo applicationInfo();

    void setupDebugMessenger();
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo();
    VkResult createDebugUtilsMessenger(VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkDebugUtilsMessengerEXT* pDebugMessenger);
    void destroyDebugUtilsMessenger(VkInstance instance,
                                    VkDebugUtilsMessengerEXT debugMessenger,
                                    const VkAllocationCallbacks* pAllocator);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData);
};

}   //  namespace spcDoomEngine

#endif  //  SPCDOOMENGINE_AINSTANCE_H
