//
//  NSString+NSStringAddition.h
//  iSing
//
//  Created by cui xiaoqian on 13-4-21.
//  Copyright (c) 2013年 iflytek. All rights reserved.
//

#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS

#import <Foundation/Foundation.h>

@interface NSString (Base64)

+ (NSString *)encodeBase64UrlWithBase64:(NSString *)strBase64;
+ (NSString *)encodeBase64WithString:(NSString *)strData;

/******************************************************************************
 函数名称 : + (NSString *)base64StringFromText:(NSString *)text
 函数描述 : 将文本转换为base64格式字符串
 输入参数 : (NSString *)text    文本
 输出参数 : N/A
 返回参数 : (NSString *)    base64格式字符串
 备注信息 :
 ******************************************************************************/
+ (NSString *)base64StringFromText:(NSString *)text;

/******************************************************************************
 函数名称 : + (NSString *)textFromBase64String:(NSString *)base64
 函数描述 : 将base64格式字符串转换为文本
 输入参数 : (NSString *)base64  base64格式字符串
 输出参数 : N/A
 返回参数 : (NSString *)    文本
 备注信息 :
 ******************************************************************************/
+ (NSString *)textFromBase64String:(NSString *)base64;

@end


@interface NSString(MD5Addition)

- (NSString *) stringFromMD5;

@end



@interface NSString (NSDate)

// 获取现在的时间str
+ (NSString *)stringWithDate:(NSDate *)date fromat:(NSString *)fromatStr;

// 把制定的时间str进行转换（与现在时间相比较）
+ (NSString *)stringWithTimeStr:(NSString *)timeStr;

- (NSDate *)date;
- (NSDate *)dateOfYear;
@end

// DES加密后，再base64
@interface NSString (DES_Base64)

- (NSString *)desBase64Encode:(NSString *)key;

- (NSString *)desBase64Unencode:(NSString *)key;

@end

#endif


