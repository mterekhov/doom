//
//  AVulkan.hpp
//  pureapp
//
//  Created by Mihail Terekhov on 13.01.2021.
//

#ifndef AVulkan_hpp
#define AVulkan_hpp

#include <stdio.h>
#include <vector>
#include <vulkan/vulkan.h>

typedef std::vector<VkExtensionProperties> TExtensionsArray;
typedef std::vector<std::string> TStringsArray;

class AVulkan {
public:
    void setViewSize(int width, int height);
    void setView(void *pView);
    void initVulkan();
    void mainLoop();
    void cleanup();
    
private:
    VkInstance instance;

    TExtensionsArray collectInstanceExtensions();
    TStringsArray collectInstanceExtensionsNames();
    void createInstance();
};

#endif /* AVulkan_hpp */
