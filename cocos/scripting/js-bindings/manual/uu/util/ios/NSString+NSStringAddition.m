//
//  NSString+NSStringAddition.m
//  iSing
//
//  Created by cui xiaoqian on 13-4-21.
//  Copyright (c) 2013年 iflytek. All rights reserved.
//
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS
#import "NSString+NSStringAddition.h"
//#import "NSData+NSDataAdditions.h"
#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>

#define IS_CH_SYMBOL(chr) ((int)(chr)>127)

//空字符串
#define     LocalStr_None           @""

static const char encodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char pinyinFirstLetter(unsigned short hanzi);
@implementation NSString (Base64)

+ (NSString *)encodeBase64UrlWithBase64:(NSString *)strBase64
{
    // Switch a base64 encoded string into a "base 64 url" encoded string
    // all that really means is no "=" padding and switch "-" for "+" and "_" for "/"
    
    // This is not a "standard" per se, but it is a widely used variant.
    // See http://en.wikipedia.org/wiki/Base64#URL_applications for more information
    
    NSString * strStep1 = [strBase64 stringByReplacingOccurrencesOfString:@"=" withString:@""];
    NSString * strStep2 = [strStep1 stringByReplacingOccurrencesOfString:@"+" withString:@"-"];
    NSString * strStep3 = [strStep2 stringByReplacingOccurrencesOfString:@"/" withString:@"_"];
    return strStep3;
}

+ (NSString *)encodeBase64WithString:(NSString *)strData {
    return [NSData encodeBase64WithData:[strData dataUsingEncoding:NSUTF8StringEncoding]];
}


+ (NSString *)base64StringFromText:(NSString *)text
{
    if (text && ![text isEqualToString:LocalStr_None]) {
        //取项目的bundleIdentifier作为KEY  改动了此处
        //NSString *key = [[NSBundle mainBundle] bundleIdentifier];
        NSData *data = [text dataUsingEncoding:NSUTF8StringEncoding];
        //IOS 自带DES加密 Begin  改动了此处
        //data = [self DESEncrypt:data WithKey:key];
        //IOS 自带DES加密 End
        return [self base64EncodedStringFrom:data];
    }
    else {
        return LocalStr_None;
    }
}

+ (NSString *)textFromBase64String:(NSString *)base64
{
    if (base64 && ![base64 isEqualToString:LocalStr_None]) {
        //取项目的bundleIdentifier作为KEY   改动了此处
        //NSString *key = [[NSBundle mainBundle] bundleIdentifier];
        NSData *data = [self dataWithBase64EncodedString:base64];
        //IOS 自带DES解密 Begin    改动了此处
        //data = [self DESDecrypt:data WithKey:key];
        //IOS 自带DES加密 End
        return [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    }
    else {
        return LocalStr_None;
    }
}

/******************************************************************************
 函数名称 : + (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
 函数描述 : 文本数据进行DES加密
 输入参数 : (NSData *)data
 (NSString *)key
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 : 此函数不可用于过长文本
 ******************************************************************************/
+ (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
{
    char keyPtr[kCCKeySizeAES256+1];
    bzero(keyPtr, sizeof(keyPtr));
    
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    
    NSUInteger dataLength = [data length];
    
    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    
    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt, kCCAlgorithmDES,
                                          kCCOptionPKCS7Padding | kCCOptionECBMode,
                                          keyPtr, kCCBlockSizeDES,
                                          NULL,
                                          [data bytes], dataLength,
                                          buffer, bufferSize,
                                          &numBytesEncrypted);
    if (cryptStatus == kCCSuccess) {
        return [NSData dataWithBytesNoCopy:buffer length:numBytesEncrypted freeWhenDone:YES];
    }
    
    free(buffer);
    return nil;
}

/******************************************************************************
 函数名称 : + (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
 函数描述 : 文本数据进行DES解密
 输入参数 : (NSData *)data
 (NSString *)key
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 : 此函数不可用于过长文本
 ******************************************************************************/
+ (NSData *)DESDecrypt:(NSData *)data WithKey:(NSString *)key
{
    char keyPtr[kCCKeySizeAES256+1];
    bzero(keyPtr, sizeof(keyPtr));
    
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    
    NSUInteger dataLength = [data length];
    
    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    
    size_t numBytesDecrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt, kCCAlgorithmDES,
                                          kCCOptionPKCS7Padding | kCCOptionECBMode,
                                          keyPtr, kCCBlockSizeDES,
                                          NULL,
                                          [data bytes], dataLength,
                                          buffer, bufferSize,
                                          &numBytesDecrypted);
    
    if (cryptStatus == kCCSuccess) {
        return [NSData dataWithBytesNoCopy:buffer length:numBytesDecrypted freeWhenDone:YES];
    }
    
    free(buffer);
    return nil;
}

/******************************************************************************
 函数名称 : + (NSData *)dataWithBase64EncodedString:(NSString *)string
 函数描述 : base64格式字符串转换为文本数据
 输入参数 : (NSString *)string
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 :
 ******************************************************************************/
+ (NSData *)dataWithBase64EncodedString:(NSString *)string
{
    if (string == nil)
        [NSException raise:NSInvalidArgumentException format:nil];
    if ([string length] == 0)
        return [NSData data];
    
    static char *decodingTable = NULL;
    if (decodingTable == NULL)
    {
        decodingTable = malloc(256);
        if (decodingTable == NULL)
            return nil;
        memset(decodingTable, CHAR_MAX, 256);
        NSUInteger i;
        for (i = 0; i < 64; i++)
            decodingTable[(short)encodingTable[i]] = i;
    }
    
    const char *characters = [string cStringUsingEncoding:NSASCIIStringEncoding];
    if (characters == NULL)     //  Not an ASCII string!
        return nil;
    char *bytes = malloc((([string length] + 3) / 4) * 3);
    if (bytes == NULL)
        return nil;
    NSUInteger length = 0;
    
    NSUInteger i = 0;
    while (YES)
    {
        char buffer[4];
        short bufferLength;
        for (bufferLength = 0; bufferLength < 4; i++)
        {
            if (characters[i] == '\0')
                break;
            if (isspace(characters[i]) || characters[i] == '=')
                continue;
            buffer[bufferLength] = decodingTable[(short)characters[i]];
            if (buffer[bufferLength++] == CHAR_MAX)      //  Illegal character!
            {
                free(bytes);
                return nil;
            }
        }
        
        if (bufferLength == 0)
            break;
        if (bufferLength == 1)      //  At least two characters are needed to produce one byte!
        {
            free(bytes);
            return nil;
        }
        
        //  Decode the characters in the buffer to bytes.
        bytes[length++] = (buffer[0] << 2) | (buffer[1] >> 4);
        if (bufferLength > 2)
            bytes[length++] = (buffer[1] << 4) | (buffer[2] >> 2);
        if (bufferLength > 3)
            bytes[length++] = (buffer[2] << 6) | buffer[3];
    }
    
    bytes = realloc(bytes, length);
    return [NSData dataWithBytesNoCopy:bytes length:length freeWhenDone:YES];
}

/******************************************************************************
 函数名称 : + (NSString *)base64EncodedStringFrom:(NSData *)data
 函数描述 : 文本数据转换为base64格式字符串
 输入参数 : (NSData *)data
 输出参数 : N/A
 返回参数 : (NSString *)
 备注信息 :
 ******************************************************************************/
+ (NSString *)base64EncodedStringFrom:(NSData *)data
{
    if ([data length] == 0)
        return @"";
    
    char *characters = malloc((([data length] + 2) / 3) * 4);
    if (characters == NULL)
        return nil;
    NSUInteger length = 0;
    
    NSUInteger i = 0;
    while (i < [data length])
    {
        char buffer[3] = {0,0,0};
        short bufferLength = 0;
        while (bufferLength < 3 && i < [data length])
            buffer[bufferLength++] = ((char *)[data bytes])[i++];
        
        //  Encode the bytes in the buffer to four characters, including padding "=" characters if necessary.
        characters[length++] = encodingTable[(buffer[0] & 0xFC) >> 2];
        characters[length++] = encodingTable[((buffer[0] & 0x03) << 4) | ((buffer[1] & 0xF0) >> 4)];
        if (bufferLength > 1)
            characters[length++] = encodingTable[((buffer[1] & 0x0F) << 2) | ((buffer[2] & 0xC0) >> 6)];
        else characters[length++] = '=';
        if (bufferLength > 2)
            characters[length++] = encodingTable[buffer[2] & 0x3F];
        else characters[length++] = '=';
    }
    
    NSString *string = [[NSString alloc] initWithBytesNoCopy:characters length:length encoding:NSASCIIStringEncoding freeWhenDone:YES];
    return string;
}

@end




@implementation NSString(MD5Addition)

- (NSString *) stringFromMD5{
    
    if(self == nil || [self length] == 0)
        return nil;
    
    const char *value = [self UTF8String];
    
    unsigned char outputBuffer[CC_MD5_DIGEST_LENGTH];
    CC_MD5(value, strlen(value), outputBuffer);
    
    NSMutableString *outputString = [[NSMutableString alloc] initWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    for(NSInteger count = 0; count < CC_MD5_DIGEST_LENGTH; count++){
        [outputString appendFormat:@"%02x",outputBuffer[count]];
    }
    
    return outputString;
}

@end

@implementation NSString (NSDate)

+ (NSString *)stringWithDate:(NSDate *)date fromat:(NSString *)fromatStr
{
    if (!date || !fromatStr)
    {
        return nil;
    }
    
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:fromatStr];
    
    return [formatter stringFromDate:date];
}

+ (NSString *)stringWithTimeStr:(NSString *)timeStr
{
    if ( !timeStr || [timeStr stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]].length == 0)
    {
        return nil;
    }
    
    if ([timeStr rangeOfString:@"+0000"].length > 0)
    {
        timeStr = [timeStr substringToIndex:[timeStr rangeOfString:@"+0000"].location];
    }
    
    // 现在时间
    NSDate *now = [NSDate date];
    
    NSString *retStr = nil;
    //格式化
    NSDateFormatter *formatter = [[NSDateFormatter alloc]init];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *mydate = [formatter dateFromString:timeStr];
    
    [formatter setDateFormat:@"MM-dd HH:mm"];
    retStr = [formatter stringFromDate:mydate];
    
    // 与现在的时间间隔
    NSTimeInterval interval = fabs([now timeIntervalSinceDate:mydate]);
    
    // 昨天和今天 肯定小于两天
    if (interval > 0 && interval < 24*60*60*2)
    {
        [formatter setDateFormat:@"DD"];
        int createdDay = [formatter stringFromDate:mydate].intValue;
        int today = [formatter stringFromDate:now].intValue;
        
        
        [formatter setDateFormat:@"HH:mm"];
        // 今天
        if (today == createdDay)
        {
            retStr = [NSString stringWithFormat:@"今天 %@",[formatter stringFromDate:mydate]];
        }
        // 昨天
        else if (createdDay + 1 == today)
        {
            retStr = [NSString stringWithFormat:@"昨天 %@",[formatter stringFromDate:mydate]];
        }
        //        else if (createdDay > today)
        //        {
        //            // 昨天
        //            if (interval < 24*60*60)
        //            {
        //                retStr = [NSString stringWithFormat:@"昨天 %@",[formatter stringFromDate:mydate]];
        //            }
        //        }
    }
    else
    {
        // 是否为同一年
        [formatter setDateFormat:@"yyyy"];
        int myYear = [formatter stringFromDate:mydate].intValue;
        int toYear = [formatter stringFromDate:now].intValue;
        // 不是同一年， 要显示全部时间
        if (myYear != toYear)
        {
            [formatter setDateFormat:@"yyyy-MM-dd HH:mm"];
            retStr = [formatter stringFromDate:mydate];
        }
    }
    
    return retStr;
}
- (NSDate *)date
{
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *date = [formatter dateFromString:self];
    return date;
}
- (NSDate *)dateOfYear
{
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    NSDate *date = [formatter dateFromString:self];
    return date;
}
@end



@implementation NSString(DES_Base64)

- (NSString *)desBase64Encode:(NSString *)key
{
    NSData *data = [NSData DesEncrypt:self withKey:key];
    NSString * str = [NSData encodeBase64WithData:data];
    return str;
}

- (NSString *)desBase64Unencode:(NSString *)key
{
    NSData *data = [NSData decodeBase64WithString:self];
    data = [NSData DESDecrypt:data WithKey:key];
    NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return str;
}
//

@end

#endif


