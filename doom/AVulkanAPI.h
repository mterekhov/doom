#ifndef SPCDOOMENGINE_AVULKANAPI_H
#define SPCDOOMENGINE_AVULKANAPI_H

#include <stdio.h>
#include "AInstance.h"

namespace spcDoomEngine {

class AVulkanAPI {
public:
    bool initVulkan(void *metalLayer, const uint32_t width, const uint32_t height);
    void destroyVulkan();

    void setViewSize(int width, int height);
    void setView(void *pView);
    void mainLoop();
    
private:
    AInstance vulkanInstance;

};

}   //  namespace spcDoomEngine

#endif  //  SPCDOOMENGINE_AVULKANAPI_H
