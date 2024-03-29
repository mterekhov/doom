//
//  GameViewController.m
//  doom
//
//  Created by Mihail Terekhov on 20.01.2021.
//

#import <MetalKit/MetalKit.h>

#import "AGameVC.h"
#import "AVulkanAPI.h"

static NSTimeInterval timeMark = 0;
static NSInteger framesCount = 0;

@interface AGameVC()

@property (nonatomic, assign) spcDoomEngine::AVulkanAPI *vulkanEngine;
@property (nonatomic, assign) BOOL canDraw;

@end

@implementation AGameVC

- (void)viewDidAppear {
    [super viewDidAppear];
    self.canDraw = NO;
    
    self.vulkanEngine = new spcDoomEngine::AVulkanAPI();
    if (!self.vulkanEngine->initVulkan((__bridge void *)self.view.layer,
                                       static_cast<uint32_t>(CGRectGetWidth(self.view.layer.bounds)),
                                       static_cast<uint32_t>(CGRectGetHeight(self.view.layer.bounds)))) {
        NSLog(@"ACHTUNG: no chance to create VULKAN instance");
        return;
    }
    self.canDraw = YES;
}
- (void)viewDidDisappear {
    [super viewDidDisappear];
    
    self.vulkanEngine->destroyVulkan();
}

- (void)dealloc
{
    self.vulkanEngine->destroyVulkan();
    delete self.vulkanEngine;
}

#pragma mark - MTKViewDelegate -

- (void)mtkView: (nonnull MTKView *)view drawableSizeWillChange: (CGSize)size
{
//    /// Respond to drawable size or orientation changes here
//
//    float aspect = size.width / (float)size.height;
//    _projectionMatrix = matrix_perspective_right_hand(65.0f * (M_PI / 180.0f), aspect, 0.1f, 100.0f);
}


- (void)drawInMTKView:(nonnull MTKView *)view {
    if (self.canDraw) {
        self.vulkanEngine->mainLoop();
    }
    NSTimeInterval diff = [NSDate new].timeIntervalSince1970;
    if (diff - timeMark > 1) {
        NSLog(@"fps: %li", (long)framesCount);
        timeMark = diff;
        framesCount = 0;
        return;
    }
    
    framesCount++;
}

@end
