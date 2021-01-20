//
//  GameViewController.m
//  doom
//
//  Created by Mihail Terekhov on 20.01.2021.
//

#import <MetalKit/MetalKit.h>

#import "AGameVC.h"

static NSTimeInterval timeMark = 0;
static NSInteger framesCount = 0;

@interface AGameVC()

//@property (nonatomic, assign)

@end

@implementation AGameVC

- (instancetype)initWithView: (NSView *)newView {
    self = [super init];
    if (self) {
        self.view = newView;
    }
    
    return self;
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
