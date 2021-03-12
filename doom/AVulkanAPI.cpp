#include "AVulkanAPI.h"

namespace spcDoomEngine {

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
    vulkanInstance.drawFrame();
}

}
