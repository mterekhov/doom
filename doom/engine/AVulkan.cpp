//
//  AVulkan.cpp
//  pureapp
//
//  Created by Mihail Terekhov on 13.01.2021.
//

#include <iostream>
#include <vector>

#include "AVulkan.h"

//static std::vector<const char *> instanceExtensions = {
//    "VK_KHR_surface",
//    "VK_EXT_metal_surface"
//};

void AVulkan::setViewSize(int width, int height) {
    
}

void AVulkan::setView(void *pView) {
    
}

void AVulkan::initVulkan() {
    createInstance();
}

void AVulkan::mainLoop() {
    
}

void AVulkan::cleanup() {
    
}

#pragma mark - Routine -

TExtensionsArray AVulkan::collectInstanceExtensions() {
    //  define count of extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    //  collect all the extensions
    TExtensionsArray extensionsList(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsList.data());

    return extensionsList;
}

TStringsArray AVulkan::collectInstanceExtensionsNames() {
    TExtensionsArray extensionsList = collectInstanceExtensions();
    TStringsArray extensionsNames = TStringsArray(extensionsList.size());
    std::cout << "available instance extensions:\n";
    for (const auto& extension : extensionsList) {
        std::cout << '\t' << extension.extensionName << '\n';
        extensionsNames.insert(extensionsNames.end(), extension.extensionName);
    }
    
    return extensionsNames;
}

void AVulkan::createInstance() {
    TStringsArray extensionsNamesList = collectInstanceExtensionsNames();

    uint32_t pApiVersion = 0;
    vkEnumerateInstanceVersion(&pApiVersion);
    
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = pApiVersion;
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
//    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsNamesList.size());
//    createInfo.ppEnabledExtensionNames = extensionsNamesList.data();

    VkResult error = vkCreateInstance(&createInfo, nullptr, &instance);
    if (error != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}
