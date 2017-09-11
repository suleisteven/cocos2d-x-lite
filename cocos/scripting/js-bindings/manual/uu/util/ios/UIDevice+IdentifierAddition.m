//
//  UIDevice(Identifier).m
//  UIDeviceAddition
//
//  Created by Georg Kitz on 20.08.11.
//  Copyright 2011 Aurora Apps. All rights reserved.
//
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS
#import "UIDevice+IdentifierAddition.h"
#import "NSString+NSStringAddition.h"

#import <AdSupport/AdSupport.h>
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>

@interface UIDevice(Private)

- (NSString *) macaddress;

@end

@implementation UIDevice (IdentifierAddition)

////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Private Methods

// Return the local MAC addy
// Courtesy of FreeBSD hackers email list
// Accidentally munged during previous update. Fixed thanks to erica sadun & mlamb.
- (NSString *) macaddress{
    
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        printf("Error: if_nametoindex error\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 1\n");
        return NULL;
    }
    
    if ((buf = malloc(len)) == NULL) {
        printf("Could not allocate memory. error!\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 2");
        free(buf);
        return NULL;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
                           *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    free(buf);
    
    return outstring;
}




+ (NSString *) uniqueGlobalDeviceIdentifier
{
    NSString *uniqueIdentifier = @"";
    if (![[UIDevice currentDevice].systemVersion floatValue] < 7.0f) {
        
        NSString *adId = nil;
        NSString *encodeFunStr = @"YWR2ZXJ0aXNpbmdJZGVudGlmaWVy";
        NSString *decodeFunStr = [NSString textFromBase64String:encodeFunStr];
        SEL idenSelector = NSSelectorFromString(decodeFunStr);
        
        NSString *encodeClassStr = @"QVNJZGVudGlmaWVyTWFuYWdlcg==";
        NSString *decodeclassStr = [NSString textFromBase64String:encodeClassStr];
        Class adManagerClass = NSClassFromString(decodeclassStr);
        id adMgr = nil;
        if ([adManagerClass respondsToSelector:@selector(sharedManager)]) {
            adMgr = [adManagerClass performSelector:@selector(sharedManager)];
        }
        
        if ([adMgr respondsToSelector:idenSelector]) {
            id uuidObj = [adMgr performSelector:idenSelector];
            if ([uuidObj respondsToSelector:@selector(UUIDString)]) {
                adId = [uuidObj performSelector:@selector(UUIDString)];
            }
        }
        if (adId && [adId isKindOfClass:[NSString class]]) {
            uniqueIdentifier = [adId stringByReplacingOccurrencesOfString:@"-" withString:@""];
        } else {
            adId = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
            uniqueIdentifier = [adId stringByReplacingOccurrencesOfString:@"-" withString:@""];
        }
        //        DLOG(@"ios7以上:%@", uniqueIdentifier);
    } else {
        NSString *macaddress = [[UIDevice currentDevice] macaddress];
        uniqueIdentifier = [macaddress stringFromMD5];
        //        DLOG(@"ios7以下:%@", uniqueIdentifier);
    }
    
    return uniqueIdentifier;
}

@end
#endif



