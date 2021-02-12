//
//  AVulkanAPI.cpp
//  pureapp
//
//  Created by Mihail Terekhov on 13.01.2021.
//

#include "AVulkanAPI.h"

namespace DoomEngine {

bool AVulkanAPI::initVulkan(void *metalLayer, const uint32_t width, const uint32_t height) {
    vulkanInstance.createInstance(metalLayer, width, height);
    return true;
}

void AVulkanAPI::destroyVulkan() {
    vulkanInstance.destroyInstance();
}

void AVulkanAPI::setViewSize(int width, int height) {
    
}

void AVulkanAPI::setView(void *pView) {
    
}

void AVulkanAPI::mainLoop() {
    
}

}
