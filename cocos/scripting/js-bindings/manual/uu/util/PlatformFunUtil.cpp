/*
 * 中文
 *
 *  Created on: 2015-03-14
 *      Author: Steven
 *   Update on:
 *       Since: Lanlord 1.0.0
 */
#include "PlatformFunUtil.h"
#include "StringUtils.h"


//===================== android ======================  
#define ANDROID_CLASS_NAME "org/cocos2dx/cpp/NativeInteractive"  

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)   

extern "C"
{
	
}
#endif  



#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
bool PlatformFunUtil::getJNIStaticMethodInfo(cocos2d::JniMethodInfo &methodinfo,
											 const char *methodName,
											 const char *paramCode) 
{
	return cocos2d::JniHelper::getStaticMethodInfo(methodinfo,
		CLASS_NAME,
		methodName,
		paramCode);
}

#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

map<string, string> PlatformFunUtil::_externalParams;

bool PlatformFunUtil::_isOpenWithOther = false;



void PlatformFunUtil::handleExternalUrl(const string& url)
{
	map<string, string> params = StringUtil::urlParamsParse(url);

	PlatformFunUtil::_externalParams = params;

	if (!params.empty())
	{
		PlatformFunUtil::_isOpenWithOther = true;
	}
	else
	{
		PlatformFunUtil::_isOpenWithOther = false;
	}
}


#endif

void PlatformFunUtil::rateUs()
{
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "rateUs", "()V")) {
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	PluginHelper::getInstance()->openURL(OnlineConfigParams::getAppDetailAppstoreUrl());
#endif


}

int PlatformFunUtil::getAppVersionCode()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getAppVersionCode", "()I")) {
		return 0;
	}

	int ret = 0;
	jint code = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	ret = (int)code;
	return ret;
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static int iosVersionCode = 0;
    
    if(iosVersionCode!=0)
    {
        return iosVersionCode;
    }
	string version = PluginHelper::getInstance()->getVersionName();
	//StringUtil::replaceAll(version, ".", "");
    vector<string> versionVec = StringUtil::split(version, '.');
    
    
    int v0 = __String(versionVec.at(0)).intValue();
    int v1 = __String(versionVec.at(1)).intValue();
    int v2 = __String(versionVec.at(2)).intValue();
    string s = StringUtils::format("%02d%02d%02d", v0,v1,v2);
    
	int ret = 0;
	if (StringUtil::isInt(s))
	{
		int code = __String(s).intValue();
        ret = code;
        iosVersionCode = ret;
	}
	return ret;
#endif

	return 0;
}

std::string PlatformFunUtil::getAppVersionName()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getAppVersionName", "()Ljava/lang/String;")) {

		return "";
	}

	std::string ret("");
	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	ret = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
	return ret;

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return PluginHelper::getInstance()->getVersionName();
#endif
	return "1.0";
}
std::string PlatformFunUtil::getAppChannel()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getAppChannel", "()Ljava/lang/String;")) {
		return "";
	}

	std::string ret("");
	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	ret = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
	return ret;

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return PluginHelper::getInstance()->getChannelName();
#endif
	return "windows";
}


void PlatformFunUtil::showToast(std::string content, bool isShowLongTime)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "showToast", "(Ljava/lang/String;Z)V")) {
		return;
	}

	jstring stringArg = methodInfo.env->NewStringUTF(content.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, isShowLongTime);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(stringArg);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	
#endif
	
}

void PlatformFunUtil::restartApp(const int& delayTime)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "restartApp", "(J)V")) {
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (jlong)delayTime);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif
}

bool PlatformFunUtil::joinQQGroup(const string& qqGroupNum, const string& key)
{
	bool result = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "joinQQGroup", "(Ljava/lang/String;)Z")) {
		return result;
	}

	jstring contentJStr = methodInfo.env->NewStringUTF(key.c_str());

	result = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, contentJStr);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(contentJStr);
	

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = PluginHelper::getInstance()->joinQQGroup(qqGroupNum, key);
	
#endif

	return result;
}

bool PlatformFunUtil::isOpenWithOther()
{
	bool result = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "isOpenWithOther", "()Z")) {
		return result;
	}

	result = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = PlatformFunUtil::_isOpenWithOther;	
#endif

	return result;
}

void PlatformFunUtil::setOpenWithOther(const bool& value)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "setOpenWithOther", "(Z)V")) {
		return;
	}
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, value);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	PlatformFunUtil::_isOpenWithOther = value;
#endif

	
	
}

string PlatformFunUtil::getExternalParam(const string& key)
{
	string result;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getExternalParam", "(Ljava/lang/String;)Ljava/lang/String;")) {
		return result;
	}
	jstring contentJStr = methodInfo.env->NewStringUTF(key.c_str());
	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,contentJStr);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(contentJStr);

	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

	auto it = PlatformFunUtil::_externalParams.find(key);

	if (it != PlatformFunUtil::_externalParams.end()){
		result =it->second;
	}
#endif

	return result;
}


int PlatformFunUtil::getBatteryPercent()
{
	int result = 0;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getBatteryPercent", "()I")) {
		return result;
	}

	jint batteryPercent = (jint)methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = (int)batteryPercent;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    result = PluginHelper::getInstance()->getBatteryPercent();
#else
	result = 100;
#endif
	return result;
}

int PlatformFunUtil::getNetType()
{
    int result = 99;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    cocos2d::JniMethodInfo methodInfo;
    
    if (! getJNIStaticMethodInfo(methodInfo, "getNetType", "()I")) {
        return result;
    }
    
    jint networktType = (jint)methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    result = (int)networktType;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    result = PluginHelper::getInstance()->getNetType();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    result = 1;
#endif
    return result;
}

std::string PlatformFunUtil::getOSType()
{
	string result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	result = "android";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = "ios";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	result =  "windows";
#endif
	return result;
}

std::string PlatformFunUtil::getOSVersion()
{
	string result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getOSVersion", "()Ljava/lang/String;")) {
		return "";
	}

	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    result =  PluginHelper::getInstance()->getOSVersion();
#else
	result = "7.0";
#endif
	return result;
}

std::string PlatformFunUtil::getNetTypeString()
{
	int type = getNetType();
	string result;
	if (type == 1)
	{
		result = "WIFI";
	}
	else if (type == 2)
	{
		result = "2G";
	}
	else if (type == 3)
	{
		result = "3G";
	}
	else if (type == 4)
	{
		result = "4G";
	}
	else
	{
		result = "UNKNOW";
	}
	return result;
}

std::string PlatformFunUtil::getDeviceModel()
{
	string result = "";

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getDeviceModel", "()Ljava/lang/String;")) {
		return "";
	}

	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result =  PluginHelper::getInstance()->getDeviceModel();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	result = "Windows7";
#endif
	return result;
}


