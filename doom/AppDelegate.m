//
//  AppDelegate.m
//  doom
//
//  Created by Mihail Terekhov on 20.01.2021.
//

#import <MetalKit/MetalKit.h>

#import "AppDelegate.h"
#import "AGameVC.h"

@interface AppDelegate ()

@property (nonatomic, weak) NSWindow *window;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
//    NSLog(@"VULKAN_SDK = %@", NSProcessInfo.processInfo.environment[@"VULKAN_SDK"]);
//    NSLog(@"VK_LAYER_PATH = %@", NSProcessInfo.processInfo.environment[@"VK_LAYER_PATH"]);
//    NSLog(@"VK_ICD_FILENAMES = %@", NSProcessInfo.processInfo.environment[@"VK_ICD_FILENAMES"]);
//    NSLog(@"PATH = %@", NSProcessInfo.processInfo.environment[@"PATH"]);
//    NSLog(@"DYLD_LIBRARY_PATH = %@", NSProcessInfo.processInfo.environment[@"DYLD_LIBRARY_PATH"]);
    self.window = [self createWindow];
    [self createMenu];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

#pragma mark - Routine -

- (NSWindow *)createWindow {
    NSSize screenSize = NSMakeSize(CGRectGetWidth(NSScreen.mainScreen.frame), CGRectGetHeight(NSScreen.mainScreen.frame));
    NSSize windowSize = NSMakeSize(800, 600);
    NSRect windowFrame = NSMakeRect(ceil(screenSize.width / 2 - windowSize.width / 2),
                                    ceil(screenSize.height / 2 - windowSize.height / 2),
                                    windowSize.width,
                                    windowSize.height);
    NSWindow *newWindow = [[NSWindow alloc] initWithContentRect: windowFrame
                                                      styleMask: NSWindowStyleMaskMiniaturizable| NSWindowStyleMaskTitled  | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
                                                        backing: NSBackingStoreBuffered
                                                          defer: NO];
    newWindow.title = [[NSProcessInfo processInfo] processName];
    [newWindow makeKeyAndOrderFront: self];
    
    MTKView *metalView = [[MTKView alloc] initWithFrame: NSMakeRect(0, 0, windowSize.width, windowSize.height)];
    metalView.device = MTLCreateSystemDefaultDevice();
    AGameVC *viewController = [AGameVC new];
    viewController.view = metalView;
    metalView.delegate = viewController;
    newWindow.contentViewController = viewController;
    
    return newWindow;
}

- (void)createMenu {
    NSString *quitTitle = [@"Quit " stringByAppendingString: [[NSProcessInfo processInfo] processName]];
    NSMenuItem *quitMenuItem = [[NSMenuItem alloc] initWithTitle: quitTitle
                                                          action: @selector(terminate:)
                                                   keyEquivalent: @"q"];
    NSMenu *appMenu = [NSMenu new];
    [appMenu addItem: quitMenuItem];
    
    NSMenuItem *appMenuItem = [NSMenuItem new];
    [appMenuItem setSubmenu: appMenu];

    NSMenu *menubar = [NSMenu new];
    [menubar addItem: appMenuItem];
    NSApp.mainMenu = menubar;
}

@end
