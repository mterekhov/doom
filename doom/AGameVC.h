//
//  GameViewController.h
//  doom
//
//  Created by Mihail Terekhov on 20.01.2021.
//

#import <Cocoa/Cocoa.h>

@protocol MTKViewDelegate;

// Our macOS view controller.
@interface AGameVC : NSViewController<MTKViewDelegate>

- (instancetype)initWithView: (NSView *)newView;

@end
