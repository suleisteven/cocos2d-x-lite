/*
 * 中文
 *
 *  Created on: 2015-03-14
 *      Author: Steven
 *   Update on:
 *       Since: Lanlord 1.0.0
 */
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS

#include "PluginHelper.h"
//#import "AppController.h"
//#import "common/constant/PublicConstant.h"

#import <UIKit/UIKit.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <Foundation/Foundation.h>
#import <UIDevice+IdentifierAddition.h>
#import <Reachability.h>
#import <sys/utsname.h>



void PluginHelper::openURL(std::string appstoreUrl) {
    
    NSString* rateURL = [NSString stringWithUTF8String:appstoreUrl.c_str()];
    
    //NSLog(@"The link to be redirected is %@",rateURL);
    [[UIApplication sharedApplication]openURL:[NSURL URLWithString:rateURL]];
 
    
//    [[UIApplication sharedApplication]openURL:[NSURL URLWithString:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=43423432432"]];
}


PluginHelper* PluginHelper::getInstance() {
    
    static PluginHelper plugin;
    
	[[UIApplication sharedApplication]setIdleTimerDisabled:YES]; // 保持屏幕常亮
    return &plugin;
    
}


void PluginHelper::onFacebookSharingButtonTouched(std::string descriptionWords, std::string imagePath, std::string appstoreUrl) {
    
    
    
    
}

//std::string PluginHelper::getVersionCode()
//{
//    return getVersionName();
//}


std::string PluginHelper::getVersionName() {
    
    NSString* plistPath = [[NSBundle mainBundle]pathForResource:@"Info" ofType:@"plist"];
    
    NSMutableDictionary* data = [[NSMutableDictionary alloc]initWithContentsOfFile:plistPath];
    
    NSString* resultString = [data objectForKey:@"CFBundleVersion"];
    
    std::string outputString = std::string([resultString UTF8String]);
    
    return outputString;
    
}

std::string PluginHelper::getChannelName() {
    
    NSString* plistPath = [[NSBundle mainBundle]pathForResource:@"Info" ofType:@"plist"];
    
    NSMutableDictionary* data = [[NSMutableDictionary alloc]initWithContentsOfFile:plistPath];
    
    NSString* resultString = [data objectForKey:@"Application Channel"];
    
    
    if(!resultString)
    {
        return "appstore";
    }
    
    
    std::string outputString = std::string([resultString UTF8String]);
    
    return outputString;
}

void PluginHelper::copyStringToClipboard(std::string content)
{

	NSString *str=[NSString stringWithUTF8String:content.c_str()];
    
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
	pasteboard.string = str;
    
}

string PluginHelper::getStringFromClipboard()
{
	return "";
}

int PluginHelper::getBatteryPercent()
{
    [UIDevice currentDevice].batteryMonitoringEnabled = YES;
    double deviceLevel = [UIDevice currentDevice].batteryLevel;
    
    int result = deviceLevel * 100;
    if(result < 0)
    {
        result = 0;
    } else if(result > 100)
    {
        result = 100;
    }
    return result;
}


string PluginHelper::getDeviceId()
{
    
    NSString *deviceIdNS = [UIDevice uniqueGlobalDeviceIdentifier];
    
    if(!deviceIdNS)
    {
        return "";
    }
    
    string result = string([deviceIdNS UTF8String]);
    
    return result;
}

int PluginHelper::getNetType()
{
    int result = 99;
    Reachability *reachability = [Reachability reachabilityWithHostName:@"www.apple.com"];
    NetworkStatus type = [reachability currentReachabilityStatus];
    switch (type) {
        case NotReachable:
            result = 99;
            break;
        case ReachableViaWiFi:
            result = 1;
            break;
        case ReachableViaWWAN:
            result = 2;
            break;
        case ReachableVia2G:
            result = 2;
            break;
        case ReachableVia3G:
            result = 3;
            break;
        case ReachableVia4G:
            result = 4;
            break;
        default:
            break;
    }
    return result;
}


string PluginHelper::getOSVersion()
{
    NSString* systemVersionNS = [UIDevice currentDevice].systemVersion;
    
    string result = string([systemVersionNS UTF8String]);
    return result;
}


string PluginHelper::getDeviceModel()
{
    //NSString* deviceModelNS = [UIDevice currentDevice].model;
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *platform = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    
    //CLog(@"%@",deviceString);
//    string result = string([deviceModelNS UTF8String]);
    string result = "iPhone";
    do
	{
		if ([platform isEqualToString:@"iPhone1,1"]) {result = "iPhone 2G";break;}
		if ([platform isEqualToString:@"iPhone1,2"]) {result = "iPhone 3G";break;}
		if ([platform isEqualToString:@"iPhone2,1"]) {result = "iPhone 3GS";break;}
		if ([platform isEqualToString:@"iPhone3,1"]) {result = "iPhone 4";break;}
		if ([platform isEqualToString:@"iPhone3,2"]) {result = "iPhone 4";break;}
		if ([platform isEqualToString:@"iPhone3,3"]) {result = "iPhone 4";break;}
		if ([platform isEqualToString:@"iPhone4,1"]) {result = "iPhone 4S";break;}
		if ([platform isEqualToString:@"iPhone5,1"]) {result = "iPhone 5";break;}
		if ([platform isEqualToString:@"iPhone5,2"]) {result = "iPhone 5";break;}
		if ([platform isEqualToString:@"iPhone5,3"]) {result = "iPhone 5C";break;}
		if ([platform isEqualToString:@"iPhone5,4"]) {result = "iPhone 5C";break;}
		if ([platform isEqualToString:@"iPhone6,1"]) {result = "iPhone 5S";break;}
		if ([platform isEqualToString:@"iPhone6,2"]) {result = "iPhone 5S";break;}
		if ([platform isEqualToString:@"iPhone7,1"]) {result = "iPhone 6 Plus";break;}
		if ([platform isEqualToString:@"iPhone7,2"]) {result = "iPhone 6";break;}
		if ([platform isEqualToString:@"iPhone8,1"]) {result = "iPhone 6S";break;}
		if ([platform isEqualToString:@"iPhone8,2"]) {result = "iPhone 6S Plus";break;}
		if ([platform isEqualToString:@"iPhone9,1"]) {result = "iPhone 7";break;}
		if ([platform isEqualToString:@"iPhone9,2"]) {result = "iPhone 7 Plus";break;}
		if ([platform isEqualToString:@"iPhone9,3"]) {result = "iPhone 7";break;}
		if ([platform isEqualToString:@"iPhone9,4"]) {result = "iPhone 7 Plus";break;}
    
		if ([platform isEqualToString:@"iPod1,1"])   {result = "iPod Touch 1G";break;}
		if ([platform isEqualToString:@"iPod2,1"])   {result = "iPod Touch 2G";break;}
		if ([platform isEqualToString:@"iPod3,1"])   {result = "iPod Touch 3G";break;}
		if ([platform isEqualToString:@"iPod4,1"])   {result = "iPod Touch 4G";break;}
		if ([platform isEqualToString:@"iPod5,1"])   {result = "iPod Touch 5G";break;}
    
		if ([platform isEqualToString:@"iPad1,1"])   {result = "iPad 1G";break;}
    
		if ([platform isEqualToString:@"iPad2,1"])   {result = "iPad 2";break;}
		if ([platform isEqualToString:@"iPad2,2"])   {result = "iPad 2";break;}
		if ([platform isEqualToString:@"iPad2,3"])   {result = "iPad 2";break;}
		if ([platform isEqualToString:@"iPad2,4"])   {result = "iPad 2";break;}
		if ([platform isEqualToString:@"iPad2,5"])   {result = "iPad Mini 1G";break;}
		if ([platform isEqualToString:@"iPad2,6"])   {result = "iPad Mini 1G";break;}
		if ([platform isEqualToString:@"iPad2,7"])   {result = "iPad Mini 1G";break;}
    
		if ([platform isEqualToString:@"iPad3,1"])   {result = "iPad 3";break;}
		if ([platform isEqualToString:@"iPad3,2"])   {result = "iPad 3";break;}
		if ([platform isEqualToString:@"iPad3,3"])   {result = "iPad 3";break;}
		if ([platform isEqualToString:@"iPad3,4"])   {result = "iPad 4";break;}
		if ([platform isEqualToString:@"iPad3,5"])   {result = "iPad 4";break;}
		if ([platform isEqualToString:@"iPad3,6"])   {result = "iPad 4";break;}
    
		if ([platform isEqualToString:@"iPad4,1"])   {result = "iPad Air";break;}
		if ([platform isEqualToString:@"iPad4,2"])   {result = "iPad Air";break;}
		if ([platform isEqualToString:@"iPad4,3"])   {result = "iPad Air";break;}
		if ([platform isEqualToString:@"iPad4,4"])   {result = "iPad Mini 2G";break;}
		if ([platform isEqualToString:@"iPad4,5"])   {result = "iPad Mini 2G";break;}
		if ([platform isEqualToString:@"iPad4,6"])   {result = "iPad Mini 2G";break;}
    
		if ([platform isEqualToString:@"i386"])      {result = "iPhone Simulator";break;}
		if ([platform isEqualToString:@"x86_64"])    {result = "iPhone Simulator";break;}
	}while(0);
  
    
    return result;
}


bool PluginHelper::joinQQGroup(const string& qqGroupNum, const string& key)
{

	NSString* qqGroupNumNSString = [NSString stringWithUTF8String:qqGroupNum.c_str()];
	NSString* keyNSString = [NSString stringWithUTF8String:key.c_str()];

	NSString *urlStr = [NSString stringWithFormat:@"mqqapi://card/show_pslcard?src_type=internal&version=1&uin=%@&key=%@&card_type=group&source=external", qqGroupNumNSString, keyNSString];
	NSURL *url = [NSURL URLWithString:urlStr];
	if([[UIApplication sharedApplication] canOpenURL:url]){
		[[UIApplication sharedApplication] openURL:url];
		return true;
	}
	else 
	{
		return false;
	}
}
#endif
