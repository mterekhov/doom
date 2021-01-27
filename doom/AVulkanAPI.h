//
//  AVulkanAPI.hpp
//  pureapp
//
//  Created by Mihail Terekhov on 13.01.2021.
//

#ifndef AVulkanAPI_hpp
#define AVulkanAPI_hpp

#include <stdio.h>
#include "AInstance.h"

namespace DoomEngine {

class AVulkanAPI {
public:
    bool initVulkan();
    void destroyVulkan();

    void setViewSize(int width, int height);
    void setView(void *pView);
    void mainLoop();
    
private:
    AInstance vulkanInstance;

};

}


#endif /* AVulkanAPI_hpp */
