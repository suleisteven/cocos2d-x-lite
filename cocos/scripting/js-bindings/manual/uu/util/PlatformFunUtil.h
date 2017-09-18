/*
 * 中文
 *
 *  Created on: 2015-03-14
 *      Author: Steven
 *   Update on:
 *       Since: Lanlord 1.0.0
 */
#ifndef __PLATFORM_FUN_UTIL_H__
#define __PLATFORM_FUN_UTIL_H__

#include "cocos2d.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#define  CLASS_NAME "org/cocos2dx/cpp/NativeInteractive" // java实现类全路径
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "PluginHelper.h"
#endif

using namespace std;
USING_NS_CC;

enum AdPosition
{
	POSITION_LEFT_TOP = 0,
	POSITION_TOP_CENTER,
	POSITION_RIGHT_TOP,
	POSITION_RIGHT_CENTER,
	POSITION_RIGHT_BOTTOM,
	POSITION_BOTTOM_CENTER,
	POSITION_LEFT_BOTTOM,
	POSITION_LEFT_CENTER
};


/************************************************************************/
/* 各平台实现功能类                                                                     */
/************************************************************************/
class PlatformFunUtil
{
public:


	/**
	 * 评价我们
	 */
	static void rateUs();

	///**
	// * 获取程序版本号(android下返回整数如100，ios下返回字符串如1.0.0)
	// */
	static int getAppVersionCode();

	/**
	 * 获取程序版本名称
	 */
	static std::string getAppVersionName();
	/**
	 * 获取程序渠道key
	 */
	static std::string getAppChannel();


	/**
	 * 获取网络类型
	 * @return 1-wifi；2-2g；3-3g；4-4g；99-未知
	 */
	static int getNetType();

	/**
	 * 获取版本类型
	 */
	static string getOSVersion();

	/**
	* 获取网络类型字符串
	*/
	static string getNetTypeString();

	/**
	* 获取系统类型
	*/
	static string getOSType();


	/**
	 * 获取设备型号
	 */
	static string getDeviceModel();

	/**
	 * 获取当前系统电量
	 * @return 0-100的值
	 */
	static int getBatteryPercent();
	/**
	 * 显示toast
	 * @param content 内容
	 * @param isShowLongTime 是否长时间显示，true-大概显示5秒，false-大概显示2.5秒
	 */
	static void showToast(std::string content, bool isShowLongTime);

	/**
	 * 重启程序
	 * @param delayTime 延时时间
	 */
	static void restartApp(const int& delayTime);

	/**
	 * 一键加入qq群
	 * @param qqGroupNum qq群号码
	 * @param key qq群key，通过http://qun.qq.com/join.html网页获取
	 */
	static bool joinQQGroup(const string& qqGroupNum, const string& key);

	/**
	 * 是否通过其他程序打开
	 */
	static bool isOpenWithOther();

	/**
	 * 设置是否从外部程序打开，当消费了外部数据后，设置此参数为false
	 */
	static void setOpenWithOther(const bool& value);

	/**
	 * 获取外部程序传入参数
	 */
	static string getExternalParam(const string& key);

    typedef std::function<void(int,string, bool)> PayCallback;
	/**
	 * iap支付
	 */
    static bool payForIAP(const string& productId, const bool& isSanboxValue, PlatformFunUtil::PayCallback callback);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	static bool getJNIStaticMethodInfo(cocos2d::JniMethodInfo &methodinfo,
		const char *methodName,
		const char *paramCode);
#endif


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	static void handleExternalUrl(const string& url);

	static map<string, string> _externalParams; // 外部参数

	static bool _isOpenWithOther;


#endif

	


};



#endif  /* __PLATFORM_FUN_UTIL_H__ */
