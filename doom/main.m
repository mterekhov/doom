//
//  main.m
//  doom
//
//  Created by Mihail Terekhov on 20.01.2021.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

int main(int argc, const char * argv[]) {
    AppDelegate *appDelegate = [AppDelegate new];
    NSApplication.sharedApplication.delegate = appDelegate;
    
    return NSApplicationMain(argc, argv);
}
