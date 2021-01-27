//
//  AVulkanAPI.cpp
//  pureapp
//
//  Created by Mihail Terekhov on 13.01.2021.
//

#include "AVulkanAPI.h"

namespace DoomEngine {

bool AVulkanAPI::initVulkan() {
    vulkanInstance.createInstance();
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
